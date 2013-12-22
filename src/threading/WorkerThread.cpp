/*
 * WorkerThread.cpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */


#include "threading/WorkerThread.hpp"

#include "threading/ThreadPool.hpp"

threading::WorkerThread::WorkerThread(ThreadPool& threadPool)
    : timeStarted()
    , threadPool(threadPool)
{
}

void threading::WorkerThread::operator ()()
{
    timeStarted = std::chrono::high_resolution_clock::now();

    Task task;

    while(!threadPool.stop)
    {
        {
            std::unique_lock<std::mutex> lock(threadPool.queueMutex);

            threadPool.waitCondition.wait(lock, [&](){ return threadPool.taskQueue.size() > 0 || threadPool.stop; });

            if (threadPool.stop)
                break;

            task = threadPool.taskQueue.top();
            threadPool.taskQueue.pop();
        }

        const auto taskStart = std::chrono::high_resolution_clock::now();

        task();

        workTime += std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - taskStart);

        if (task.flags & Task::Flags::REENTRANT)
            threadPool.enqueue(std::move(task));

        if (threadPool.isSynchronizing)
            threadPool.syncCondition.notify_one();
    }
}
