#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "protocol/auth.grpc.pb.h"
#include "protocol/items.grpc.pb.h"

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

using game::battlemon::items::ItemsService;
using game::battlemon::items::PlayersItemsRequest;
using game::battlemon::items::PlayersItemsResponse;
using game::battlemon::items::PlayerItems;
using game::battlemon::items::SetMyItemsRequest;
using game::battlemon::common::Empty;

#define HOOK_ERROR char*& error, void(*errorH)(const std::string& copy, char*& error)

class gRPC_SSL
{
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

public:
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

template <typename Service, typename ServiceStub>
class gRPC_Client : public gRPC_SSL
{
protected:
	std::unique_ptr<ServiceStub> stub;
	std::string error;

public:
	gRPC_Client()
	{
		stub = std::unique_ptr<ServiceStub>(Service::NewStub((grpc::CreateChannel("game.battlemon.com", grpc::SslCredentials(getSslOptions())))));
	}
	~gRPC_Client() {}
};

class gRPC_ClientAuth : public gRPC_Client<AuthService, AuthService::Stub>
{
    bool GetOneCode(const SendCodeRequest& write, SendCodeResponse* read);
    bool GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read);

public:

    SendCodeResponse CallRPCSendCode(const std::string& publicKey, HOOK_ERROR);
    VerifyCodeResponse CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, HOOK_ERROR);
};


class gRPC_ClientItems : public gRPC_Client<ItemsService, ItemsService::Stub>
{
	bool GetOnePlayersItems(const PlayersItemsRequest& write, PlayersItemsResponse* read);
	bool GetOneSetMyItems(SetMyItemsRequest& write, Empty* read);

public:
	
	PlayerItems CallRPC_GetPlayersItems(const std::string& room_id, int id_Player, const std::string& near_ids, HOOK_ERROR);
	void CallRPC_SetMyItems(const std::string& room_id, int id_Player, const std::string& nft_ids, HOOK_ERROR);
};