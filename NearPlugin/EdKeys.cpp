#define _CRT_SECURE_NO_WARNINGS 1
#include "EdKeys.h"
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#include "src/ed25519.h"
#include <filesystem>
#ifdef _WIN32
#include <Windows.h>

#elif defined(__unix__) || defined(__APPLE__)
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#define SSIZE_T ssize_t
#endif
#ifdef __APPLE__
#include <codecvt>
#endif

#include <iostream>
#include <vector>
#include <fstream>

uint8_t* base64_encode(const uint8_t* src, size_t len, size_t* out_len)
{
	const uint8_t base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint8_t* out, * pos;
	const uint8_t* end, * in;
	size_t olen;
	int line_len;

	olen = len * 4 / 3 + 4;
	olen += olen / 72;
	olen++; 
	if (olen < len)
		return NULL; 
	out = (uint8_t*)malloc(olen);
	if (out == NULL)
		return NULL;

	end = src + len;
	in = src;
	pos = out;
	line_len = 0;
	while (end - in >= 3)
	{
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (line_len >= 72) {
			//*pos++ = '\n';
			line_len = 0;
		}
	}
	bool b = end - in;
	if (end - in)
	{
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		}
		else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) |
				(in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	//if (line_len)
	//	*pos++ = '\n';

	*pos = '\0';
	if (out_len)
		*out_len = pos - out;
	return out;
}

void memzero(void* const pnt, const size_t len) 
{
#ifdef _WIN32
	SecureZeroMemory(pnt, len);
#elif defined(HAVE_MEMSET_S)
	memset_s(pnt, (rsize_t)len, 0, (rsize_t)len);
#elif defined(HAVE_EXPLICIT_BZERO)
	explicit_bzero(pnt, len);
#elif defined(HAVE_EXPLICIT_MEMSET)
	explicit_memset(pnt, 0, len);
#else
	volatile unsigned char* volatile pnt_ = (volatile unsigned char* volatile)pnt;
	size_t i = (size_t)0U;

	while (i < len) {
		pnt_[i++] = 0U;
	}
#endif
}

bool b58enc(char* b58, size_t* b58sz, const void* data, size_t binsz) 
{
	const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	const uint8_t* bin = (uint8_t*)data;
	int carry;
	SSIZE_T i, j, high, zcount = 0;
	size_t size;

	while (zcount < (SSIZE_T)binsz && !bin[zcount]) ++zcount;

	size = (binsz - zcount) * 138 / 100 + 1;
	b58 = new char[size];
	uint8_t *buf = new uint8_t[size];
	memzero(buf, size);

	for (i = zcount, high = size - 1; i < (SSIZE_T)binsz; ++i, high = j) {
		for (carry = bin[i], j = size - 1; (j > high) || carry; --j) {
			carry += 256 * buf[j];
			buf[j] = carry % 58;
			carry /= 58;
		}
	}
	
	for (j = 0; j < (SSIZE_T)size && !buf[j]; ++j)
		;

	if (*b58sz <= zcount + size - j) {
		*b58sz = zcount + size - j + 1;
		return false;
	}

	if (zcount) memset(b58, '1', zcount);
	for (i = zcount; j < (SSIZE_T)size; ++i, ++j)
		b58[i] = b58digits_ordered[buf[j]];
	b58[i] = '\0';
	*b58sz = i + 1;

	return true;
}

TYPE_STRING EncodeBase58(const uint8_t* dataIn, const size_t &sizeData)
{
	const uint8_t base58map[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	const uint8_t* data = dataIn;
	std::vector<uint8_t> digits((sizeData * 138 / 100) + 1);
	size_t digitslen = 1;
	for (size_t i = 0; i < sizeData; i++)
	{
		uint32_t carry = static_cast<uint32_t>(data[i]);
		for (size_t j = 0; j < digitslen; j++)
		{
			carry = carry + static_cast<uint32_t>(digits[j] << 8);
			digits[j] = static_cast<uint8_t>(carry % 58);
			carry /= 58;
		}
		for (; carry; carry /= 58)
			digits[digitslen++] = static_cast<uint8_t>(carry % 58);
	}
	TYPE_STRING result = _T("ed25519:");
	for (size_t i = 0; i < (sizeData - 1) && !data[i]; i++)
		result.push_back(base58map[0]);
	for (size_t i = 0; i < digitslen; i++)
		result.push_back(base58map[digits[digitslen - 1 - i]]);
	return result;
}

EdKeys::EdKeys():state(false)
{
	public_key[0] = '\0';
	private_key[0] = '\0';
	sign.resize(89);
}

EdKeys::~EdKeys()
{
}

void EdKeys::GeneratingKeys(TYPE_CHAR*& error, void(*errorH)(const TYPE_STRING& copy, TYPE_CHAR* &error))
{
	unsigned char seed[32];
	if (ed25519_create_seed(seed))
	{
		errorH(_T("Invalid seed"), error);
		return;
	}
	state = true;
	ed25519_create_keypair(public_key, private_key, seed);
}

const char* EdKeys::MessageSigning(const char* messageInp)
{
	unsigned char signature[64];
	std::string message = messageInp;
	ed25519_sign(signature, (const uint8_t*)message.c_str(), message.size(), public_key, private_key);
	std::string sign = (const char*)signature;
	sign.resize(64);
	size_t sizeOut;
	this->sign = (const char*)base64_encode(signature, sign.size(), &sizeOut);
	return this->sign.c_str();
}


void EdKeys::SaveK(const TYPE_STRING &filename, void* key, size_t size)
{
#ifdef __APPLE__
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string utf16_string(filename);
	std::string UTF8 = convert.to_bytes(utf16_string);
#else
	std::ofstream outfile(filename, std::ofstream::binary);
#endif
	outfile.write((const char*)&size, sizeof(size));
	outfile.write((const char*)key, size);
	outfile.close();
}

void EdKeys::SaveKeys(const TYPE_STRING& accountID, TYPE_STRING dir)
{
    if(dir != _T(""))
    {
        TYPE_STRING pashProject = dir;
        SaveK(pashProject + accountID + _T(".pr.bin"), private_key, 64);
        SaveK(pashProject + accountID + _T(".pb.bin"), public_key, 32);
		SaveK(pashProject + accountID + _T(".sign.pb.bin"), (void*)sign.c_str(), 89);
    }
    else
    {
        SaveK(TYPE_STRING(_T("/")) + _T("saved") + accountID + _T(".pr.bin"), private_key, 64);
        SaveK(TYPE_STRING(_T("/")) + _T("saved") + accountID + _T(".pb.bin"), public_key, 32);
		SaveK(TYPE_STRING(_T("/")) + _T("saved") + accountID + _T(".sign.pb.bin"), (void*)sign.c_str(), 89);
    }							 
}

bool EdKeys::LoadK(const TYPE_STRING& path, const TYPE_STRING& accountID, const TYPE_STRING& filetype, void* key, size_t size)
{
	TYPE_STRING filename = path + accountID + filetype;
#ifdef __APPLE__
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string utf16_string(filename);
	std::string UTF8 = convert.to_bytes(utf16_string);
#else
	std::ifstream infile(filename, std::ofstream::binary);
#endif
	if (infile)
	{
		infile.read((char*)&size, sizeof(size_t));
		infile.read((char*)key, size);
		infile.close();
		return true;
	}
	return false;
}

bool EdKeys::LoadKeys(const TYPE_STRING& accountID, TYPE_STRING dir)
{
    
    if(dir != _T(""))
    {
        TYPE_STRING pashProject = dir;
        if (LoadK(pashProject, accountID, _T(".pr.bin"), private_key, 64))
        {
            LoadK(pashProject, accountID, _T(".pb.bin"), public_key, 32);
			LoadK(pashProject, accountID, _T(".sign.pb.bin"), (TYPE_CHAR*)sign.c_str(), 89);
			state = true;
            return true;
        }
    }
    else
        if (LoadK((TYPE_STRING(_T("/")) + _T("saved")), accountID, _T(".pr.bin"), private_key, 64))
        {
            LoadK((TYPE_STRING(_T("/")) + _T("saved")), accountID, _T(".pb.bin"), public_key, 32);
			LoadK((TYPE_STRING(_T("/")) + _T("saved")), accountID, _T(".sign.pb.bin"), (TYPE_CHAR*)sign.c_str(), 89);
			state = true;
            return true;
        }
    
	return false;
}

void EdKeys::SaveSign(const TYPE_STRING& accountID, TYPE_STRING dir, const char* sign)
{
	this->sign = sign;
	SaveK(TYPE_STRING(_T("/")) + _T("saved") + accountID + _T(".sign.pb.bin"), (void*)sign, 89);
}

bool EdKeys::LoadSign(const TYPE_STRING& accountID, TYPE_STRING dir)
{
	if(LoadK((TYPE_STRING(_T("/")) + _T("saved")), accountID, _T(".sign.pb.bin"), (void*)sign.c_str(), 89))
		return true;
	return false;
}

TYPE_STRING EdKeys::GetPubKey58() const
{
	return EncodeBase58(public_key, 32);
}

TYPE_STRING EdKeys::GetPrKey58() const
{
	return EncodeBase58(private_key, 64);
}

const std::string& EdKeys::GetSign() const
{
	return sign;
}

void EdKeys::SetSign(const std::string& sign)
{
	this->sign = sign;
}

bool EdKeys::IsValid() const
{
	return state;
}
