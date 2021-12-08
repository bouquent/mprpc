#ifndef LOGQUEUE_H
#define LOGQUEUE_H
#include <pthread.h>
#include <queue>
#include <stdlib.h>

template<typename T>
class LockQueue
{
public:
    LockQueue()
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }
    ~LockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
    LockQueue(const LockQueue&) = delete;
    LockQueue& operator=(const LockQueue&) = delete;
    T pop()
    {
        if (lockQueue_.empty()) {
            throw "lockQueue is empty!";
            exit(EXIT_FAILURE);
        } 
        pthread_mutex_lock(&mutex_);
        while (lockQueue_.empty()) {
            pthread_cond_wait(&cond_, &mutex_);
        }
        T tmp = lockQueue_.front();
        lockQueue_.pop();
        pthread_mutex_unlock(&mutex_);
        return tmp;
    }
    T& front()
    {
        return lockQueue_.front();
    }
    void push(const T& a)
    {
        pthread_mutex_lock(&mutex_);
        lockQueue_.push(a);
        pthread_mutex_unlock(&mutex_);

        pthread_cond_broadcast(&cond_);
    }
    
    bool empty() 
    {
        return lockQueue_.empty();
    }
private:
    std::queue<T> lockQueue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};


#endif
