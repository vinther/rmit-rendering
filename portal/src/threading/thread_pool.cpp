#include "threading/thread_pool.hpp"

#include "threading/worker_thread.hpp"

#include "shared/utilities.hpp"

threading::thread_pool::thread_pool()
    : stop(false)
{
}

threading::thread_pool::~thread_pool()
{
    stop = true;

    waitCondition.notify_all();

    for (auto& thread: workerThreads) {
        thread.join();
    }
}

bool threading::thread_pool::initialize()
{
    for (auto& workerThread: workerThreads)
    {
        workerThread.join();
    }

    for (unsigned int i = 0; i < settings.numThreads; ++i)
    {
        workerThreads.push_back(std::thread(worker_thread(*this)));
    }

    return true;
}

void threading::thread_pool::enqueue(task&& task)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        taskQueue.push(task);
    }

    waitCondition.notify_one();
}

void threading::thread_pool::enqueue(std::vector<task>& tasks)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        for (task task: tasks)
            taskQueue.push(std::move(task));
    }

    waitCondition.notify_all();
}

void threading::thread_pool::synchronize(task::Flags flags)
{
    UNUSED(flags);
    // TODO Enable use of synchronization flags

    {
        std::unique_lock<std::mutex> lock(queueMutex);

        isSynchronizing = true;

        syncCondition.wait(lock, [&]() {
            return taskQueue.size() == 0;
        });

        isSynchronizing = false;
    }
}


