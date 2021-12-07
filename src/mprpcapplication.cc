#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>

MprpcConfig MprpcApplication::m_config;

void showArgsHelp()
{
    std::cout << "format: command -i <configure>" << std::endl;
}

void MprpcApplication::Init(int argc, char** argv)
{
    if (argc < 2) {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    char c = '\0';
    std::string config_file;
    while ((c = ::getopt(argc, argv, "i:")) != -1) {
        switch (c) {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            std::cout << "need <configure>" << std::endl;
            showArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    //加载配置文件config_file  rpcserver_ip=...   zookeeper_ip=...
    m_config.LoadConfigFile(config_file.c_str());
}
    
MprpcApplication* MprpcApplication::getInstance() {
    static MprpcApplication rpc;
    return &rpc;
}

MprpcConfig* MprpcApplication::getConfig()
{
    return &m_config;
}