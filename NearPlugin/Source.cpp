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
        //Request = MakeSendCodeRequest("ed25519:5BoNdi92bYvwiEJBw6RGidrLNz6hbkBcnBiVxxrZAPsM");
        ClientContext context;
        GetOneCode(write, &read);
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

        return status.ok();
    }
    bool GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
    {
        ClientContext context;
        Status status = stub->VerifyCode(&context, write, read);
        std::cout << "SendCode rpc: " << status.ok() << std::endl;

        return status.ok();
    }

    SendCodeRequest MakeSendCodeRequest(const std::string& publicKey)
    {
        SendCodeRequest s;
        s.set_public_key(publicKey);
        return s;
    }

    VerifyCodeRequest MakeFeature(const std::string& publicKey, const std::string& sign) {
        VerifyCodeRequest v;
        v.set_public_key(publicKey);
        v.set_sign(sign);
        return v;
    }
    std::unique_ptr<AuthService::Stub> stub;
};


//grpc_connectivity_state
//typedef enum {
//    /** channel is idle */
//    GRPC_CHANNEL_IDLE,
//    /** channel is connecting */
//    GRPC_CHANNEL_CONNECTING,
//    /** channel is ready for work */
//    GRPC_CHANNEL_READY,
//    /** channel has seen a failure but expects to recover */
//    GRPC_CHANNEL_TRANSIENT_FAILURE,
//    /** channel has seen a failure that it cannot recover from */
//    GRPC_CHANNEL_SHUTDOWN
//};

std::shared_ptr<Channel> channelF()
{
    //return grpc::CreateChannel("https://game.battlemon.com", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("https://game.battlemon.com/near", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("https://game.battlemon.com/near/", grpc::InsecureChannelCredentials());

    //return grpc::CreateChannel("https://23.22.240.113:80", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("https://23.22.240.113:80/near", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("https://23.22.240.113:80/near/", grpc::InsecureChannelCredentials());

    //return grpc::CreateChannel("http://game.battlemon.com", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("http://game.battlemon.com/near", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("http://game.battlemon.com/near/", grpc::InsecureChannelCredentials());

    //return grpc::CreateChannel("http://23.22.240.113:80", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("http://23.22.240.113:80/near", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("http://23.22.240.113:80/near/", grpc::InsecureChannelCredentials());

    //return grpc::CreateChannel("game.battlemon.com", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("game.battlemon.com/near", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("game.battlemon.com/near/", grpc::InsecureChannelCredentials());

    //return grpc::CreateChannel("23.22.240.113:80", grpc::InsecureChannelCredentials());
    //return grpc::CreateChannel("23.22.240.113:80/near", grpc::InsecureChannelCredentials());
    return grpc::CreateChannel("23.22.240.113:80/near/", grpc::InsecureChannelCredentials());
}


int main() 
{
    std::shared_ptr<Channel> channel = channelF();
    grpc_connectivity_state state = channel->GetState(true);
    std::cout << "MainState: " << state << std::endl;
    AuthServiceClient client(channel);

    state = channel->GetState(true);
    std::cout << "MainState: " << state << std::endl;

    //AuthServiceClient client(grpc::CreateCustomChannel();
    SendCodeResponse read = client.CallRPCSendCode("ed25519:5BoNdi92bYvwiEJBw6RGidrLNz6hbkBcnBiVxxrZAPsM");
    std::cout << "MainReadCode" << read.code() << std::endl;
    state = channel->GetState(true);
    std::cout << "MainState: " << state << std::endl;

    read = client.CallRPCSendCode("5BoNdi92bYvwiEJBw6RGidrLNz6hbkBcnBiVxxrZAPsM");
    std::cout << "MainReadCode" << read.code() << std::endl;
    state = channel->GetState(true);
    std::cout << "MainState: " << state << std::endl;


    return 0;
}