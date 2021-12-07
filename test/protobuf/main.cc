#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{   
    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);
    
    User *user1 = rsp.add_friend_list();
    user1->set_name("czz");
    user1->set_age("18");
    user1->set_sex(User::MAN);
    
    User *user2 = rsp.add_friend_list();
    user2->set_name("llr");
    user2->set_age("21");
    user2->set_sex(User::MAN);

    //std::cout << rsp.friend_list_size() << std::endl;
    User User1 = rsp.friend_list(1);
    std::cout << User1.name() << "  " << User1.age() << "  " << User1.sex() << std::endl;
    
    return 0;
}

/*LoginResponse rsp;
    将string字节流反序列化
    ResultCode* sp = rsp.mutable_result();
    sp->set_errcode(2);
    sp->set_errmsg("登录失败!");
    std::string s;
    if (rsp.SerializeToString(&s)) {
        std::cout << s << std::endl;
    }*/
/*LoginRequest req;

    将对象序列化为string字节流
    req.set_name("zhang san");
    req.set_pwd("123456");
    std::string s;
    if (req.SerializeToString(&s)) {
        std::cout << s << std::endl;
    }*/