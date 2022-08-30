#pragma once
#include <memory>
#include <thread>
#include <atomic>

#include "Utils/SafeQueue.hpp"

template <typename T>
class Worker
{
    std::shared_ptr<SafeQueue<T>> _queue;
    std::thread _thread;
    std::atomic<bool> _stop_token;

public:
    explicit Worker(std::shared_ptr<SafeQueue<T>> queue)
    :
    _queue(std::move(queue)),
    _thread(&Worker::run, this),
    _stop_token(false)
    {
    }

    ~Worker()
    {
        _stop_token = true;
        _thread.join();
    }

private:
    void run()
    {
        while(!_stop_token)
        {
            auto optional_task = _queue->pop();

            if(optional_task)
            {
                (*optional_task)();
            }
        }
    }
};