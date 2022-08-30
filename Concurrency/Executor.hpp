#pragma once
#include <future>
#include <memory>

#include "Utils/SafeQueue.hpp"
#include "Worker.hpp"

class Executor
{
    typedef std::packaged_task<void()> Task;

    std::shared_ptr<SafeQueue<Task>> _tasks;
    Worker<Task> _worker;

public:
    static Executor& getInstance()
    {
        static Executor executor;
        return executor;
    }

    std::future<void> post(std::function<void()>&& task)
    {
        Task packaged_task(std::move(task));
        auto task_future = packaged_task.get_future();

        _tasks->push(std::move(packaged_task));

        return task_future;
    }

private:
    Executor()
    :
    _tasks(std::make_shared<SafeQueue<Task>>()),
    _worker(_tasks)
    {
    }
};