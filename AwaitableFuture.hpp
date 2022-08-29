#pragma once
#include <future>
#include <thread>
#include <experimental/coroutine>
#include <type_traits>
#include <chrono>

#include "Concurrency/Executor.hpp"

template <typename T>
struct std::experimental::coroutine_traits<std::future<T>>
{
    struct promise_type
    {
        std::promise<T> _promise;

        std::future<T> get_return_object() noexcept
        {
            return _promise.get_future();
        }

        std::experimental::suspend_never initial_suspend() const noexcept
        { return {}; }

        std::experimental::suspend_never final_suspend() const noexcept
        { return {}; }

        void return_value(const T& value)
        {
            _promise.set_value(value);
        }

        void return_value(T&& value)
        {
            _promise.set_value(std::move(value));
        }

        void unhandled_exception() noexcept
        {
            _promise.set_exception(std::current_exception());
        }
    };
};

template <typename T>
auto operator co_await(std::future<T> future) noexcept
{
    struct FutureAwaiter : std::future<T>
    {
        bool await_ready() noexcept
        {
            return this->wait_for(std::chrono::seconds(0))
                == std::future_status::ready;
        }

        void await_suspend(std::experimental::coroutine_handle<> coroutine_handle) noexcept
        {
            Executor::getInstance().post([this, coroutine_handle]() mutable
            {
                this->wait();
                coroutine_handle();
            });
        }

        T await_resume() noexcept
        {
            return this->get();
        }
    };

    return FutureAwaiter { std::move(future) };
};