#ifndef RPCPROVIDER_H
#define RPCPROVIDER_H
#include "mprpcapplication.h"
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <memory>
#include <string>
#include <unordered_map>

class RpcProvider
{
public:
    void NotifyService(google::protobuf::Service* service);
    void Run();
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp time);
    //处理好业务后的回调函数，执行返回数据的反序列化以及打包发送
    void rpcSendResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response);

private:
    std::unique_ptr<muduo::net::TcpServer> tcpServer_;
    muduo::net::EventLoop mainloop_;

    //一个服务一个serviceinfo，里面有service以及对应的method方法
    struct serviceInfo{
        google::protobuf::Service* service_;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap_;
    };
    std::unordered_map<std::string, serviceInfo> serverMap_; 
};

#endif