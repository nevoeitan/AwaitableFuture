#include <iostream>

#include "AwaitableFuture.hpp"

std::future<int> func()
{
    auto res = co_await std::async([]
            {
                std::this_thread::sleep_for(std::chrono::seconds(3));

                return 30;
            });

    co_return res;
}

int main()
{
    auto coro = func();

    while(coro.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
    {
        std::cout << "Not ready\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << coro.get();

    return 0;
}
