#include "EdKeys.h"
#include <stdexcept>
#include <sys/types.h>
#include <cstring>

#include "src/ed25519.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __unix__
#include <strings.h>
#include <sys/param.h>
typedef SSIZE_T ssize_t;
#endif

#include <iostream>
#include <vector>
#include <fstream>

uint8_t* base64_encode(const uint8_t* src, size_t len, size_t* out_len)
{
	const uint8_t base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	 //const unsigned char base58_table[65] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
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
			*pos++ = '\n';
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

	if (line_len)
		*pos++ = '\n';

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

std::string EncodeBase58(const uint8_t* dataIn, const size_t &sizeData)
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
	std::string result = "ed25519:";
	for (size_t i = 0; i < (sizeData - 1) && !data[i]; i++)
		result.push_back(base58map[0]);
	for (size_t i = 0; i < digitslen; i++)
		result.push_back(base58map[digits[digitslen - 1 - i]]);
	return result;
}

EdKeys::EdKeys()
{
	public_key[0] = {};
	private_key[0] = {};
}

EdKeys::~EdKeys()
{
}

void EdKeys::GeneratingKeys()
{
	unsigned char seed[32];
	unsigned char signature[64];

	if (ed25519_create_seed(seed))
	{
		throw std::runtime_error("Invalid seed");
	}
	ed25519_create_keypair(public_key, private_key, seed);
}

std::string EdKeys::MessageSigning(const std::string &messageInp)
{
	unsigned char signature[64];
	ed25519_sign(signature, (const uint8_t*)messageInp.c_str(), messageInp.size(), public_key, private_key);
	std::string sign = (const char*)signature;
	sign.resize(64);
	size_t sizeOut;
	std::string sign64 = (const char*)base64_encode(signature, sign.size(), &sizeOut);
	return sign64;
}

void EdKeys::SaveK(std::string filename, void* key, size_t size)
{
	std::ofstream outfile(filename, std::ofstream::binary);
	outfile.write((const char*)&size, sizeof(size));
	outfile.write((const char*)key, size);
	outfile.close();
}

void EdKeys::SaveKeys(std::string accountID, std::string network)
{
	SaveK(accountID + network + std::string("pr.bin"), private_key, 64);
	SaveK(accountID + network + std::string("pb.bin"), public_key, 32);
}

void EdKeys::LoadK(std::string filename, void* key, size_t size)
{
	std::ifstream infile(filename, std::ofstream::binary);
	if (infile)
	{
		infile.read((char*)&size, sizeof(size_t));
		infile.read((char*)key, size);
		infile.close();
		return;
	}
	throw std::runtime_error("Invalid load " + filename);
}

void EdKeys::LoadKeys(std::string accountID, std::string network)
{
	LoadK(accountID + network + std::string("pr.bin"), private_key, 64);
	LoadK(accountID + network + std::string("pb.bin"), public_key, 32);
}

std::string EdKeys::GetPubKey58() const
{
	return EncodeBase58(public_key, 32);
}

