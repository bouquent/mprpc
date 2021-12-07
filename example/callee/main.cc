#include <string>
#include <iostream>
#include "user.pb.h"
#include "friend.pb.h"
#include "friendservice.h"
#include "userservice.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

int main(int argc, char** argv)
{
    //调用框架的初始化操作
    MprpcApplication::Init(argc, argv);


    //provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());
    provider.NotifyService(new FriendService());

    //启动rpc服务发布节点，进进程进入阻塞，等待远程的rpc调用请求
    provider.Run();
    return 0;
}