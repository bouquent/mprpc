#ifndef LOGGING_H
#define LOGGING_H
#include "lockqueue.h"
#include <string>
#include <iostream>

enum logLevel
{
    INFO = 1,
    ERROR,
    DEBUG,
    FATAL,
};

class Logging
{
public:
    static Logging* getInstance();
    static void* writeWork(void *v);

    void setLogLevel(logLevel level_);

    void log(const std::string& msg);
private:
    Logging();
private:
    LockQueue<std::string> logQueue_;
    logLevel logLevel_;
};


#define LOG_INFO(logformat, ...) \
    do {\
        Logging* logging = Logging::getInstance();\
        logging->setLogLevel(INFO);\
        char msg[1024] = {0};\
        snprintf(msg, sizeof(msg), logformat, ##__VA_ARGS__);\
        logging->log(msg);\
    	std::cout << msg << std::endl;\
    } while (0); 

#define LOG_ERROR(logformat, ...) \
    do {\
        Logging* logging = Logging::getInstance();\
        logging->setLogLevel(ERROR);\
        char msg[1024] = {0};\
        snprintf(msg, sizeof(msg), logformat, ##__VA_ARGS__);\
        logging->log(msg);\
    } while (0); 

#define LOG_FATAL(logformat, ...) \
    do {\
        Logging* logging = Logging::getInstance();\
        logging->setLogLevel(FATAL);\
        char msg[1024] = {0};\
        snprintf(msg, sizeof(msg), logformat, ##__VA_ARGS__);\
        logging->log(msg);\
        exit(1);\
    } while (0); 

#endif
