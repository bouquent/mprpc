#include "include/logging.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>

Logging::Logging()
{
    pthread_t tid;
    //开启一个线程专门用来写日志
    pthread_create(&tid, nullptr, &Logging::writeWork, this);
    pthread_detach(tid);
}

//写日志线程
void* Logging::writeWork(void *v)
{
    Logging* log =  (Logging*)v;
    while (1) {
        if (!log->logQueue_.empty()) {
            time_t t = time(NULL);
            struct tm* tm_time;
            tm_time = localtime(&t);
            char logfile[128] = {0};
            sprintf(logfile, "../log/%04d-%02d-%02d.txt", 
                            tm_time->tm_year + 1900,
                            tm_time->tm_mon + 1,
                            tm_time->tm_mday);
            char logtime[128] = {0};
            sprintf(logtime, "%02d:%02d:%02d  ", 
                            tm_time->tm_hour,
                            tm_time->tm_min,
                            tm_time->tm_sec);
        //没有则创建，只能追加写
	    std::cout << "logfile:" << logfile << "logtime:" << logtime << std::endl;
	    FILE *fp = fopen(logfile, "a+");
            if (fp == nullptr) {
                std::cout << "open " << logfile << " failure" << std::endl;
                return nullptr;
            }
            std::string msg = log->logQueue_.pop();
            msg = std::string(logtime) + msg + "\n";
            fputs(msg.c_str(), fp);
            fclose(fp);
        }
    }
}

Logging* Logging::getInstance()
{
    static Logging logging;
    return &logging;
}
void Logging::setLogLevel(logLevel level_)
{
    logLevel_ = level_;
}
void Logging::log(const std::string& msg)
{
    logQueue_.push(msg);
}