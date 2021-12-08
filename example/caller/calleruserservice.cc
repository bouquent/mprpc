#include <iostream>
#include "user.pb.h"
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"
#include "logging.h"

int main(int argc, char** argv) 
{
    MprpcApplication::Init(argc, argv);
    fixbug::FriendListRpc_Stub stub(new MprpcChannel());
    
    MprpcController controller;
    fixbug::GetFriendListRequest request;
    fixbug::GetFriendListResponse response;
    request.set_id(10);
    stub.GetFriendList(&controller, &request, &response, nullptr);

    if (controller.Failed()) {
        LOG_ERROR("rpc service is expection!");
    } else {
        if (0 == response.result().errcode()) {
            for (int i = 0; i < response.friends_size(); ++i) {
                std::cout << response.friends(i) << std::endl;
            }
        std::cout << std::endl;
        } else {
            LOG_ERROR("rpc getfriendlist response success:%s",response.result().errmsg().c_str());
        }
    }
    return 0;
}

#if 0
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    //请求对象
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    fixbug::LoginResponse response; //相应结果
    stub.Login(nullptr, &request, &response, nullptr);

    if (0 == response.result().errcode()) {
        std::cout << "rpc login response success : " << response.success() << std::endl;;
    } else {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest registerRequest;
    fixbug::RegisterResponse registerResponse;
    registerRequest.set_name("lisi");
    registerRequest.set_pwd("654321");
    stub.Register(nullptr, &registerRequest, &registerResponse, nullptr);
    if (0 == registerResponse.result().errcode()) {
        std::cout << "registerResponse success:" << registerResponse.success() << std::endl;
    } else {
        std::cout << "registerResponse error" << response.result().errmsg() << std::endl;
    }
#endif