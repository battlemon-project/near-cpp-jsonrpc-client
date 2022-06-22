#include "GRPC_Client.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>


GRPC_Client::GRPC_Client(std::shared_ptr<Channel> channel) : stub(AuthService::NewStub(channel))
{
}

void GRPC_Client::setChannel(std::shared_ptr<Channel> channel)
{
    stub = AuthService::NewStub(channel);
}

SendCodeResponse GRPC_Client::CallRPCSendCode(const std::string& publicKey)
{
    SendCodeRequest write;
    SendCodeResponse read;
    write.set_public_key(publicKey);
    GetOneCode(write, &read);
    return read;
}

VerifyCodeResponse GRPC_Client::CallRPCVerifyCode(const std::string& publicKey, const std::string& sign)
{
    VerifyCodeRequest write;
    VerifyCodeResponse read;
    write.set_public_key(publicKey);
    write.set_sign(sign);
    GetOneVerify(write, &read);

    return read;
}

bool GRPC_Client::GetOneCode(const SendCodeRequest& write, SendCodeResponse* read)
{
    ClientContext context;

    Status status = stub->SendCode(&context, write, read);

#ifdef _DEBUG
    if (status.ok())
    {
        return status.ok();
    }
    else
    {
        std::cout << "error_message: " << status.error_message() << std::endl;
        std::cout << "error_details: " << status.error_details() << std::endl;
        // lets print the error code, which is 3
        std::cout << "error_code: " << status.error_code() << std::endl;
    }
#endif
    return status.ok();
}

bool GRPC_Client::GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
{
    ClientContext context;
    Status status = stub->VerifyCode(&context, write, read);

#ifdef _DEBUG
    if (status.ok())
    {
        return status.ok();
    }
    else
    {
        std::cout << "error_message: " << status.error_message() << std::endl;
        std::cout << "error_details: " << status.error_details() << std::endl;
        // lets print the error code, which is 3
        std::cout << "error_code: " << status.error_code() << std::endl;
    }
#endif
    return status.ok();
}