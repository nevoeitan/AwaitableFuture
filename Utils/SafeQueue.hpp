#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <optional>

template<typename T>
class SafeQueue
{
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition_variable;

    static constexpr auto TIMEOUT = std::chrono::milliseconds(100);
public:
    template<typename... Args>
    void emplace(Args&&... args)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.emplace(std::forward<Args...>(args...));
        }

        _condition_variable.notify_one();
    }

    void push(const T& value)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(value);
        }

        _condition_variable.notify_one();
    }

    void push(T&& value)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(value));
        }

        _condition_variable.notify_one();
    }
    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition_variable.wait_for(lock, TIMEOUT, [this]{ return !_queue.empty(); });

        if(_queue.empty())
        {
            return {};
        }

        auto value = std::forward<T>(_queue.front());
        _queue.pop();

        return { std::forward<T>(value) };
    }
};