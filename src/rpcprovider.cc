#include "rpcprovider.h"
#include "rpcheader.pb.h"
#include <functional>


//发布rpc方法的函数接口,将发布的服务和方法存在本地
//记录发布的rpc服务，以及服务中所有的rcp方法，以便在方便定位远端请求的rcp服务以及方法是哪一个
void RpcProvider::NotifyService(google::protobuf::Service* service)
{
    serviceInfo service_info;
    service_info.service_ = service;

    //获取服务对象的描述柄
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    //通过描述柄获取服务对象的名字
    std::string service_name = pserviceDesc->name();
    //通过描述柄获取对象的方法数量
    int methodCnt = pserviceDesc->method_count();

    for (int i = 0; i < methodCnt; ++i) {
        //加入所有定义的rpc方法以及名字
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.methodMap_.insert({method_name, pmethodDesc});
    }

    serverMap_.insert({service_name, service_info});
}

//rpc服务开始运行
void RpcProvider::Run()
{
    std::string serverip = MprpcApplication::getInstance()->getConfig()->load("rpcserverip");
    std::string serverport = MprpcApplication::getInstance()->getConfig()->load("rpcserverport");

    muduo::net::InetAddress address(serverip.c_str(), atoi(serverport.c_str()));
    muduo::net::TcpServer tcpServer(&mainloop_, address, "rpcProvider");

    tcpServer.setConnectionCallback(std::bind(&RpcProvider::onConnection, this, std::placeholders::_1));
    tcpServer.setMessageCallback(std::bind(&RpcProvider::onMessage, this, std::placeholders::_1,
                               std::placeholders::_2, std::placeholders::_3));

    tcpServer.setThreadNum(3);
    //根据设置的线程数量开启subloop(one loop per thread)
    tcpServer.start();

    //启动主loop
    mainloop_.loop();    
}

void RpcProvider::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
    if (!conn->connected()) {
        //对端关闭了连接
        conn->shutdown();
    }
}
void RpcProvider::onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp time)
{
    std::string recv_buf = buffer->retrieveAllAsString();

    //规定头4个字节是数据包头大小,解决粘包问题
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    //根据header_size大小读取数据头的原始字符流，然后将数据头信息反序列化后得到参数数据所占字节大小即args_size
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;
    std::string server_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str)) {
        server_name = rpcHeader.server_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        std::cout << "rpc_header_str: " << rpc_header_str << " prase wrong!" << std::endl;
        return ;
    }
    //在本地查找对应的方法和服务
    auto it = serverMap_.find(server_name);
    if (serverMap_.end() == it) {
        std::cout << "not find the service!" << std::endl;
        return ;
    }
    serviceInfo serviceinfo = it->second;
    auto mit = serviceinfo.methodMap_.find(method_name);
    if (serviceinfo.methodMap_.end() == mit) {
        std::cout << "not find the service!" << std::endl;
        return ;
    }
    //获取相应的service以及method描述柄
    google::protobuf::Service *service = it->second.service_;
    const google::protobuf::MethodDescriptor *method = mit->second;


    //protobuf生成的所有类都由message继承而来，给相应的函数生成对应的参数
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();
    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    if (!request->ParseFromString(args_str)) {
        std::cout << "requset parse error, content: " << args_str << std::endl;
        return ;
    }
    std::cout << "args_str :"<< args_str << std::endl;
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    //设置准备好数据后的回调函数
    google::protobuf::Closure *done = 
        google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>
            (this, &RpcProvider::rpcSendResponse, conn, response);
    

    //调用客户想要调用的函数 Login()controller, request, response, done
    service->CallMethod(method, nullptr, request, response, done);   
}


//处理好业务后的回调函数，执行返回数据的反序列化以及打包发送
void RpcProvider::rpcSendResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
{
    std::string response_str; 

    //序列化发送给客户
    if (response->SerializeToString(&response_str)) {
        conn->send(response_str);
    } else {             
        std::cout << "SerializeTostring wrong!\n" << std::endl;
    }

    //实现短连接，无论是否成功都断开连接
    conn->shutdown();
}