#include "user.pb.h"
#include <string>
#include "logging.h"

class UserService : public fixbug::UserServiceRpc //rpc服务的提供者
{
public:
    bool Login(std::string name, std::string pwd) {
        LOG_INFO("doing local service: Login");
        LOG_INFO("name: %s,pwd: %s", name.c_str(), pwd.c_str());
        return true; 
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
       std::cout << "do local register" << std::endl;
       std::cout << "id" << id << " name:" << name << " pwd" << pwd << std::endl;
       return true;
    }

    /*重写基类UserServiceRpc的虚函数 
        1 caller ===> Login(LoginRequest) => muduo => callee
        2 callee ===> login(loginRequest) 交到下面重写的这个Login方法上
        3 通过callee调用本地业务，并返回数据
    这个函数是框架来调用的
    */
    void Login(::google::protobuf::RpcController* controller, 
                    const ::fixbug::LoginRequest* request,
                    ::fixbug::LoginResponse* response,
                    ::google::protobuf::Closure* done)
    {
        //1 获取请求业务参数
        std::string name = request->name();
        std::string pwd = request->pwd();
        
        //2 做本地业务
        bool login_result = Login(name, pwd);    

        //3 把响应数据写入
        fixbug::ResultCode *result = response->mutable_result();
        result->set_errcode(0);
        if (!login_result) {
            result->set_errmsg("login_result!");
        } else {
            result->set_errmsg(" ");
        }
        response->set_success(login_result);

        //执行响应数据的序列化和网络发送
        done->Run();
    }
    void Register(::google::protobuf::RpcController *controcall, 
                    const fixbug::RegisterRequest *request,
                    fixbug::RegisterResponse *response,
                    ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();


        bool register_result = Register(id, name, pwd);
        response->set_success(register_result);
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg(" ");
        
        done->Run();
    }
private:
};
