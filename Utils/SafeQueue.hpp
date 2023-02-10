#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <optional>

template<std::movable T>
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

    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition_variable.wait_for(lock, TIMEOUT, [this]{ return !_queue.empty(); });

        if(_queue.empty())
        {
            return {};
        }

        auto value = std::move(_queue.front());
        _queue.pop();

        return { std::move(value) };
    }
};