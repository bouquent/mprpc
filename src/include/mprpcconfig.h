#ifndef MPRPCCONFIG_H
#define MPRPCCONFIG_H

#include <unordered_map>
#include <string>

//rpcserverip   rpcserverport  zookeeperip zookeeperport 
//读取框架配置文件类
class MprpcConfig
{
public:
    void LoadConfigFile(const char* configfile);

    std::string load(const std::string&);
private:
    std::unordered_map<std::string, std::string> m_configMap;
};

#endif