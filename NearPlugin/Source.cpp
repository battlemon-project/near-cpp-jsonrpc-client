#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

//#define GRPC_ARG_ENABLE_HTTP_PROXY 0
//#define GRPC_DEFAULT_SSL_ROOTS_FILE_PATH_ENV_VAR = "J:\source\GitRepos\near-cpp-jsonrpc-client\packages\grpc\etc"
#define GRPC_VERBOSITY DEBUG
#define GRPC_TRACE tcp
#define GRPC_SSL_TARGET_NAME_OVERRIDE_ARG "grpc.ssl_target_name_override" 

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

        int size = 1024*32;
        char* c = new char[size];
        Status status = stub->SendCode(&context, write, //read);
            (SendCodeResponse*)c);

        (char*)c;
        for (size_t i = 0; i < size* 2; i++)
        {
            std::cout << c[i];
        }
        delete[]c;

        std::cout << "SendCode rpc: " << status.ok() << std::endl;
        if (status.ok()) 
        {
            return status.ok();
        }
        else 
        {
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

#include <wincrypt.h>
#include <Windows.h>

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
    // Fetch root certificate as required on Windows (s. issue 25533).
    grpc::SslCredentialsOptions result;

    // Open root certificate store.
    HANDLE hRootCertStore = CertOpenSystemStoreW(NULL, L"ROOT");
    if (!hRootCertStore)
        return result;

    // Get all root certificates.
    PCCERT_CONTEXT pCert = NULL;
    while ((pCert = CertEnumCertificatesInStore(hRootCertStore, pCert)) != NULL)
    {
        // Append this certificate in PEM formatted data.
        DWORD size = 0;
        CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, NULL, &size);
        std::vector<WCHAR> pem(size);
        CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, pem.data(), &size);

        result.pem_root_certs += utf8Encode(pem.data());
    }
    CertCloseStore(hRootCertStore, 0);

    return result;
}

int main() 
{
    try
    {
        auto channel_creds = grpc::experimental::TlsCredentials(grpc::experimental::TlsChannelCredentialsOptions());
        //auto channel_creds = grpc::SslCredentials(getSslOptions());
        std::shared_ptr<Channel> channel = grpc::CreateChannel("game.battlemon.com:80", channel_creds);
        //std::shared_ptr<Channel> channel = grpc::CreateChannel("23.22.240.113:80", grpc::InsecureChannelCredentials());
  
        std::unique_ptr<AuthService::Stub> stub(AuthService::NewStub(channel));
        SendCodeRequest write;
        SendCodeResponse read;
        write.set_public_key("ed25519:5BoNdi92bYvwiEJBw6RGidrLNz6hbkBcnBiVxxrZAPsM");
        ClientContext context;

        std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
        grpc::Status status = stub->SendCode(&context, write, &read);
        if (status.ok())
        {
        }
        else
        {
            std::cout << "error_message: " << status.error_message() << std::endl;
            std::cout << "error_details: " << status.error_details() << std::endl;
        }

        std::cout << std::endl << "context.debug_error_string: " << context.debug_error_string() << std::endl;

        //std::shared_ptr<Channel> channel = grpc::CreateChannel("game.battlemon.com:80", grpc::InsecureChannelCredentials());
        //AuthServiceClient client(channel);
        //
        ////AuthServiceClient client(grpc::CreateCustomChannel();
        //SendCodeResponse read = client.CallRPCSendCode("ed25519:5BoNdi92bYvwiEJBw6RGidrLNz6hbkBcnBiVxxrZAPsM");
        std::cout << "MainReadCode" << read.code() << std::endl << std::endl;

        //std::cout << "GetServiceConfigJSON: " << v->GetServiceConfigJSON() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }


    return 0;
}

