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

SendCodeResponse GRPC_Client::CallRPCSendCode(const std::string& publicKey, char*& error, void(*errorH)(const std::string& copy, char*& error))
{
    SendCodeRequest write;
    SendCodeResponse read;
    write.set_public_key(publicKey);
    if(GetOneCode(write, &read))
    {
        errorH(this->error, error);
    }
    return read;
}

VerifyCodeResponse GRPC_Client::CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, char*& error, void(*errorH)(const std::string& copy, char*& error))
{
    VerifyCodeRequest write;
    VerifyCodeResponse read;
    write.set_public_key(publicKey);
    write.set_sign(sign);
    if (GetOneVerify(write, &read))
    {
        errorH(this->error, error);
    }

    return read;
}

bool GRPC_Client::GetOneCode(const SendCodeRequest& write, SendCodeResponse* read)
{
    ClientContext context;

    Status status = stub->SendCode(&context, write, read);

    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}

bool GRPC_Client::GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
{
    ClientContext context;
    Status status = stub->VerifyCode(&context, write, read);


    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}