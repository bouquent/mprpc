#include "mprpcconfig.h"
#include <iostream>

//去除前后空格
void Trim(std::string& s) 
{
    int left = 0, right = s.size() - 1;
    while (left < s.size() && s[left] == ' ') left++;
    while (right > left && s[right] == ' ') right--;
    s = s.substr(left, right - left + 1);
}

void MprpcConfig::LoadConfigFile(const char* configfile)
{
    FILE *pf = fopen(configfile, "r");
    if (nullptr == pf) {
        std::cout << configfile << " is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    //1 注释  2正确的配置项  3. 去掉开头的空格
    char buf[64] = {0};
    while (fgets(buf, sizeof(buf), pf)) {
        std::string s = buf;
        int idx = s.find('=');
        if (idx == -1) {
            continue;
        }
        std::string key = s.substr(0, idx);
        Trim(key);
        std::string value = s.substr(idx + 1, s.size() - idx - 2);
        Trim(value);
        m_configMap.insert({key, value});
    }
}

std::string MprpcConfig::load(const std::string& s)
{
    auto iter = m_configMap.find(s);
    if (iter == m_configMap.end()) {
        //没有这个配置信息
        return "";
    }
    return iter->second;
}