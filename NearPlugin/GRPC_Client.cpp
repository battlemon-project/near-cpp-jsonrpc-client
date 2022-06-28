#include "GRPC_Client.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>


GRPC_Client::GRPC_Client():stub(nullptr)
{
}

GRPC_Client::GRPC_Client(std::shared_ptr<Channel> channel, Protocol type)
{
    setChannel(channel, type);
}

void GRPC_Client::setChannel(std::shared_ptr<Channel> channel, Protocol type)
{
    switch (type)
    {
    case AUTHS:
        std::unique_ptr< AuthService::Stub>* stub = new std::unique_ptr< AuthService::Stub>(AuthService::NewStub(channel));
        this->stub = stub;
        break;
    case INTERNALAUTHS:
        std::unique_ptr< InternalAuthService::Stub>* stub = new std::unique_ptr< InternalAuthService::Stub>(InternalAuthService::NewStub(channel));
        this->stub = stub;
        break;
    default:
        break;
    }

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

InternalVerifySignResponse GRPC_Client::CallInternalAuthService(std::string near_account_id, std::string sing, char*& error, void(*errorH)(const std::string& copy, char*& error))
{
    InternalVerifySignRequest write;
    InternalVerifySignResponse read;
    write.set_near_account_id(near_account_id);
    write.set_sign(sing);
    if (GetVerifySign(write, &read))
    {
        errorH(this->error, error);
    }
    return read;
}

bool GRPC_Client::GetOneCode(const SendCodeRequest& write, SendCodeResponse* read)
{
    ClientContext context;
    std::unique_ptr< AuthService::Stub>* stub = (std::unique_ptr<game::battlemon::auth::AuthService::Stub, std::default_delete<game::battlemon::auth::AuthService::Stub>>*)this->stub;
    Status status = stub->get()->SendCode(&context, write, read);

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
    std::unique_ptr< AuthService::Stub>* stub = (std::unique_ptr<AuthService::Stub, std::default_delete<AuthService::Stub>>*)this->stub;
    Status status = stub->get()->VerifyCode(&context, write, read);


    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}

bool GRPC_Client::GetVerifySign(const InternalVerifySignRequest& write, InternalVerifySignResponse* read)
{
    ClientContext context;
    std::unique_ptr< InternalAuthService::Stub>* stub = (std::unique_ptr<InternalAuthService::Stub, std::default_delete<InternalAuthService::Stub>>*)this->stub;
    Status status = stub->get()->InternalVerifySign(&context, write, read);

    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}
