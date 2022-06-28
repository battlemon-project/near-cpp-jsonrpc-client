#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "protocol/auth.grpc.pb.h"
#include "protocol/internalAuth.grpc.pb.h"


#ifdef WIN32

#include <wincrypt.h>
#include <Windows.h>
#include <cryptuiapi.h>
#ifdef _DEBUG
#pragma comment (lib, "crypt32.lib")
#endif
#endif // WIN32



using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using game::battlemon::auth::SendCodeRequest;
using game::battlemon::auth::SendCodeResponse;
using game::battlemon::auth::VerifyCodeRequest;
using game::battlemon::auth::VerifyCodeResponse;
using game::battlemon::auth::AuthService;
using game::battlemon::auth::internal::InternalVerifySignRequest;
using game::battlemon::auth::internal::InternalVerifySignResponse;
using game::battlemon::auth::internal::InternalAuthService;


enum class Protocol
{
	AUTHS,
	INTERNALAUTHS,
};

class GRPC_Client
{
    //std::unique_ptr<AuthService::Stub> stub;
	void* stub;
    bool GetOneCode(const SendCodeRequest& write, SendCodeResponse* read);
    bool GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read);
	bool GetVerifySign(const InternalVerifySignRequest& write, InternalVerifySignResponse* read);

public:
	std::string error;
	GRPC_Client();
    GRPC_Client(std::shared_ptr<Channel> channel, Protocol type);
	~GRPC_Client();

	void setChannel(std::shared_ptr<Channel> channel, Protocol type);

    SendCodeResponse CallRPCSendCode(const std::string& publicKey, char*& error, void(*errorH)(const std::string& copy, char*& error));
    VerifyCodeResponse CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, char*& error, void(*errorH)(const std::string& copy, char*& error));


	InternalVerifySignResponse CallInternalAuthService(std::string near_account_id, std::string sing, char*& error, void(*errorH)(const std::string& copy, char*& error));

#ifdef WIN32
	std::string utf8Encode(const std::wstring& wstr)
	{
		if (wstr.empty())
			return std::string();

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
		return strTo;
	}


	grpc::SslCredentialsOptions getSslOptions()
	{
		grpc::SslCredentialsOptions result;

		HANDLE hRootCertStore = CertOpenSystemStoreW(NULL, L"ROOT");
		if (!hRootCertStore)
			return result;

		PCCERT_CONTEXT pCert = NULL;
		while ((pCert = CertEnumCertificatesInStore(hRootCertStore, pCert)) != NULL)
		{
			DWORD size = 0;
			CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, NULL, &size);
			std::vector<WCHAR> pem(size);
			CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, pem.data(), &size);

			result.pem_root_certs += utf8Encode(pem.data());
		}
		CertCloseStore(hRootCertStore, 0);

		return result;
	}
#else
	grpc::SslCredentialsOptions getSslOptions()
	{
		return grpc::SslCredentialsOptions();
	}
#endif

};


