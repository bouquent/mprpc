#include "friend.pb.h"
#include <vector>
#include <iostream>
#include <string>

class FriendService : public fixbug::FriendListRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t id)
    {
        std::cout << "local GetFriendList" << std::endl;
        std::vector<std::string> vec;
        vec.push_back("czz");
        vec.push_back("llr");
        vec.push_back("uuu");
        return vec;
    }

    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //获取参数
        uint32_t id = request->id();
        //调用本地业务
        std::vector<std::string> vecFriend = GetFriendList(id);

        ::fixbug::Resultcode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg(" ");
        for (auto friendstr : vecFriend) {
            response->add_friends(friendstr);
        }

        done->Run();
    }
};