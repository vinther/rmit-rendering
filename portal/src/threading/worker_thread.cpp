#include "threading/worker_thread.hpp"

#include "threading/thread_pool.hpp"

threading::worker_thread::worker_thread(thread_pool& parent_pool)
    : timeStarted()
    , parent_pool(parent_pool)
{
}

void threading::worker_thread::operator ()()
{
    timeStarted = std::chrono::high_resolution_clock::now();

    task task;

    while(!parent_pool.stop)
    {
        {
            std::unique_lock<std::mutex> lock(parent_pool.queueMutex);

            parent_pool.waitCondition.wait(lock, [&](){ return parent_pool.taskQueue.size() > 0 || parent_pool.stop; });

            if (parent_pool.stop)
                break;

            task = parent_pool.taskQueue.top();
            parent_pool.taskQueue.pop();
        }

        const auto taskStart = std::chrono::high_resolution_clock::now();

        task();

        workTime += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - taskStart);

        if (task.flags & task::Flags::REENTRANT)
            parent_pool.enqueue(std::move(task));

        if (parent_pool.isSynchronizing)
            parent_pool.syncCondition.notify_one();
    }
}
