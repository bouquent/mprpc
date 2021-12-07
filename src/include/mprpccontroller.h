#ifndef MPRPCCONTROLLER_H
#define MPRPCCONTROLLER_H
#include <google/protobuf/service.h>

class MprpcController : public google::protobuf::RpcController
{
public:
    MprpcController();

    void Reset();

    bool Failed() const;

    std::string ErrorText() const;

    void SetFailed(const std::string& reason);


    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool failed_ = false;
    std::string errText_;
};

#endif
 