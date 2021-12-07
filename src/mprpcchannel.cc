#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory>

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                     google::protobuf::RpcController *controller,
                     const google::protobuf::Message *request,
                     google::protobuf::Message *response,
                     google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //参数大小
    int args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str)) {
        args_size = args_str.size();
    } else {
        controller->SetFailed("serialize request wrong!");
        return ;
    }

    //定义rpc的请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_server_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        controller->SetFailed("sserialize rpc_header_str wrong!");
        return ;
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout << "service_name :" << service_name << std::endl;
    std::cout << "method_name :" << method_name << std::endl;
    std::cout << "args_size :" << args_size << std::endl;
    std::cout << "args_str:" << args_str << std::endl;
    std::cout << "=================================" << std::endl;

    //获取服务端ip和地址
    std::string ip = MprpcApplication::getInstance()->getConfig()->load("rpcserverip");
    std::string port = MprpcApplication::getInstance()->getConfig()->load("rpcserverport");
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(clientfd != -1);
    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(port.c_str()));
    ser_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    int ret = connect(clientfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    assert(ret == 0);
    ret = send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0);
    if (ret < 0 && errno == EINTR) {
        close(clientfd);
        controller->SetFailed("send wrong!");
        return ;
    }

    //接受服务端响应回来的数据,并将数据打包成reponse(message)给客户
    char recv_buf[1024] = {0}; 
    ret = recv(clientfd, recv_buf, sizeof(recv_buf), 0);
    if (ret < 0 && errno == EINTR) {
        close(clientfd);
        controller->SetFailed("recv wrong!");
        return ;
    }
    std::string recv_buf_str(recv_buf, ret);
    if (!response->ParseFromString(recv_buf)) {
        close(clientfd);
        controller->SetFailed("ParseFromString wrong!");
        return ;
    }
}
