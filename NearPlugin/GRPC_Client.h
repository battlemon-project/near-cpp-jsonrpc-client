#pragma once
#include "Helper.h"
#include "include/ModelItem.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "protocol/auth.grpc.pb.h"
#include "protocol/items.grpc.pb.h"
#include "protocol/internalMm.grpc.pb.h"
#include "protocol/mm.grpc.pb.h"
#include "protocol/updates.grpc.pb.h"


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
using game::battlemon::items::GetBundlesRequest;
using game::battlemon::items::GetBundlesResponse;
using game::battlemon::items::WeaponBundle;
using game::battlemon::items::EditBundleRequest;
using game::battlemon::items::AttachBundleRequest;
using game::battlemon::items::DetachBundleRequest;
using game::battlemon::items::ItemsResponse;
using game::battlemon::items::ItemsRequest;
using game::battlemon::common::Empty;

using game::battlemon::mm::MMService;
using game::battlemon::mm::AcceptGameRequest;
using game::battlemon::mm::SearchGameResponseStatus;
using game::battlemon::mm::SearchGameResponse;
using game::battlemon::mm::AcceptGameRequest;
using game::battlemon::mm::CancelSearchRequest;
using game::battlemon::mm::SearchGameRequest;
using game::battlemon::mm::GameMode;


using game::battlemon::mm::internal::InternalMMService;
using game::battlemon::mm::internal::InternalUserLeftBattleRequest;
using game::battlemon::mm::internal::SaveBattleResultRequest;
using game::battlemon::mm::internal::RoomInfoRequest;
using game::battlemon::mm::internal::RoomInfoResponse;
using game::battlemon::mm::internal::CreateRoomRequest;
using game::battlemon::mm::internal::DedicatedServerIsReadyRequest;

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
public:
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

	void CreateContext(ClientContext & context,std::string meta_key[], std::string meta_value[], const int number)
	{
		for (size_t i = 0; i < number; i++)
		{
			context.AddMetadata(meta_key[i], meta_value[i]);
		}
	}

public:
	gRPC_Client(const bool& ssl, std::string url = "game.battlemon.com")
	{
		if(ssl)
			stub = std::unique_ptr<ServiceStub>(Service::NewStub((grpc::CreateChannel(url, grpc::SslCredentials(getSslOptions())))));
		else
			stub = std::unique_ptr<ServiceStub>(Service::NewStub((grpc::CreateChannel(url, grpc::InsecureChannelCredentials()))));
	}

	~gRPC_Client() {}
};

class gRPC_ClientAuth : public gRPC_Client<AuthService, AuthService::Stub>
{
	//bool CallRPC(ClientContext& context, const SendCodeRequest& write, SendCodeResponse* read);
	//bool CallRPC(ClientContext& context, const VerifyCodeRequest& write, VerifyCodeResponse* read);

public:
	gRPC_ClientAuth(const bool& ssl, std::string url);
	SendCodeResponse CallRPCSendCode(const std::string& publicKey, HOOK_ERROR);
	VerifyCodeResponse CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, HOOK_ERROR);
};


class gRPC_ClientItems : public gRPC_Client<ItemsService, ItemsService::Stub>
{

public:
	gRPC_ClientItems(const bool& ssl, std::string url);

	ItemsResponse CallRPC_GetItems(const std::string& nearID, const std::string& sign, HOOK_ERROR);
	GetBundlesResponse CallRPC_GetBundles(const std::string& nearID, const std::string& sign, HOOK_ERROR);
	WeaponBundle CallRPC_EditBundle(const std::string& nearID, const std::string& sign, ModelItems::EditBundleRequest& request, HOOK_ERROR);
	bool CallRPC_AttachBundle(const std::string& nearID, const std::string& sign, ModelItems::AttachBundleRequest& request, HOOK_ERROR);
	bool CallRPC_DetachBundle(const std::string& nearID, const std::string& sign, ModelItems::DetachBundleRequest& request, HOOK_ERROR);
};

class gRPC_ClientMM : public gRPC_Client<MMService, MMService::Stub>
{

public:
	gRPC_ClientMM(const bool& ssl, std::string url);
	SearchGameResponse CallRPC_SearchGame(const std::string& nearID, const std::string& sign, ModelMM::SearchGameRequest& Request, HOOK_ERROR);
	bool CallRPC_AcceptGame(const std::string& nearID, const std::string& sign, ModelMM::AcceptGameRequest& Request, HOOK_ERROR);
	bool CallRPC_CancelSearch(const std::string& nearID, const std::string& sign, HOOK_ERROR);
};

class gRPC_ClientInternalMM : public gRPC_Client<InternalMMService, InternalMMService::Stub>
{
public:
	gRPC_ClientInternalMM(const bool& ssl, std::string url);
	bool CallRPC_UserLeftBattle(ModelInternalMM::InternalUserLeftBattleRequest& Request, HOOK_ERROR);
	bool CallRPC_SaveBattleResult(ModelInternalMM::SaveBattleResultRequest& Request, HOOK_ERROR);
	RoomInfoResponse CallRPC_GetRoomInfo(ModelInternalMM::RoomInfoRequest& Request, HOOK_ERROR);
	RoomInfoResponse CallRPC_CreateRoomWithPlayers(ModelInternalMM::CreateRoomRequest& Request, HOOK_ERROR);
	bool CallRPC_DedicatedServerIsReady(ModelInternalMM::DedicatedServerIsReadyRequest& Request, HOOK_ERROR);
};



class ClientUpdates
{

public:
	ClientUpdates(const void* out, const ModelUpdates::MessageData& message);
};