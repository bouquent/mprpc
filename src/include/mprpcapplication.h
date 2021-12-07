#ifndef MPRPCAPPLICATION_H
#define MPRPCAPPLICATION_H
#include "mprpcconfig.h"

//框架基础类
class MprpcApplication
{
public:
    static void Init(int argc, char** argv);
    
    //获取MprpcApplication对象唯一接口
    static MprpcApplication* getInstance();
    static MprpcConfig* getConfig();
private:
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
    
    //加载配置文件的类
    static MprpcConfig m_config;
};

#endif