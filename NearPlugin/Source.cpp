#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#ifdef BAZEL_BUILD
#include "examples/protos/route_guide.grpc.pb.h"
#else
#include "protocol/auth.grpc.pb.h"
#endif



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




class AuthServiceClient {
public:
    AuthServiceClient(std::shared_ptr<Channel> channel): stub(AuthService::NewStub(channel))
    {
    }

    SendCodeResponse CallRPCSendCode(const std::string& publicKey)
    {
        SendCodeRequest write;
        SendCodeResponse read;
        write.set_public_key(publicKey);
        ClientContext context;
        int SIZE = 8192;
        char* c = new char[SIZE];
        (void*)c;

        GetOneCode(write, (SendCodeResponse*)c);
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
        return read;
    }
    VerifyCodeResponse CallRPCVerifyCode(const std::string& publicKey, const std::string& sign)
    {
        VerifyCodeRequest write;
        VerifyCodeResponse read;
        write.set_public_key(publicKey);
        write.set_sign(sign);
        GetOneVerify(write, &read);

        return read;
    }

private:
    bool GetOneCode(const SendCodeRequest& write, SendCodeResponse* read)
    {
        ClientContext context;

        Status status = stub->SendCode(&context, write, read);

        std::cout << "SendCode rpc: " << status.ok() << std::endl;
        if (status.ok()) 
        {
            return status.ok();
        }
        else 
        {
#ifdef _DEBUG
            // ouch!
            // lets print the gRPC error message
            // which is "Length of `Name` cannot be more than 10 characters"
            std::cout << "error_message: " << status.error_message() << std::endl;
            std::cout << "error_details: " << status.error_details() << std::endl;
            // lets print the error code, which is 3
            std::cout << "error_code: " << status.error_code() << std::endl;
            // want to do some specific action based on the error?
            if (status.error_code() == grpc::StatusCode::INVALID_ARGUMENT) 
            {
                // do your thing here
            }
            return status.ok();
#endif
        }
    }

    bool GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
    {
        ClientContext context;
        Status status = stub->VerifyCode(&context, write, read);
        std::cout << "SendCode rpc: " << status.ok() << std::endl;

        return status.ok();
    }

    std::unique_ptr<AuthService::Stub> stub;
};
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
#include "EdKeys.h"
#include <ctime>
int main() 
{
    try
    {
        EdKeysInterfase EdKeys;
        EdKeys.LoadKeys("dsbgfnghcjhgds", "testnet");

        std::shared_ptr<grpc::ChannelCredentials> channel_creds = grpc::SslCredentials(getSslOptions());
        std::shared_ptr<Channel> channel = grpc::CreateChannel("game.battlemon.com", channel_creds);
  
        std::unique_ptr<AuthService::Stub> stub(AuthService::NewStub(channel));
        SendCodeRequest write;
        SendCodeResponse read;
        VerifyCodeRequest writeV;
        VerifyCodeResponse readV;
        char* key = EdKeys.GetStrPubKey();
        write.set_public_key(key);

        ClientContext context;
        ClientContext contextV;

        grpc::Status status = stub->SendCode(&context, write, &read);
        unsigned int start_time = clock();

        write.set_public_key(key);
        writeV.set_sign(EdKeys.MessageSigning(read.code().c_str()));
        status = stub->VerifyCode(&contextV, writeV, &readV);
        unsigned int end_time = clock();
        unsigned int search_time = end_time - start_time;

        std::cout << "End VerifyCode: " << search_time << std::endl << std::endl;
        if (status.ok())
        {
            std::cout << "near_account_id: " << readV.near_account_id() << std::endl << std::endl;
        }
        else
        {
            std::cout << "error: " << readV.near_account_id() << std::endl;
            std::cout << "error_message: " << status.error_message() << std::endl;
            std::cout << "error_details: " << status.error_details() << std::endl;
            std::cout << "context.debug_error_string: " << contextV.debug_error_string() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

