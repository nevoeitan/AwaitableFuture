# AwaitableFuture

This project shows how std::future can be used with co_await.
In this implementation the await for the result happens in the backgronud in a threadpool.

## Usage

To use the code include AwaitableFuture.hpp

## Example
Here is an exmaple from main.cpp:
```
std::future<int> func()
{
    auto res = co_await std::async([]
            {
                std::this_thread::sleep_for(std::chrono::seconds(3));

                return 30;
            });

    co_return res;
}
```
