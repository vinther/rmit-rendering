/*
 * ThreadPool.cpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#include "threading/ThreadPool.hpp"

#include "threading/WorkerThread.hpp"

#include "Utilities.hpp"

threading::ThreadPool::ThreadPool()
    : stop(false)
{
}

threading::ThreadPool::~ThreadPool()
{
    stop = true;

    waitCondition.notify_all();

    for (auto& thread: workerThreads) {
        thread.join();
    }
}

bool threading::ThreadPool::initialize()
{
    for (auto& workerThread: workerThreads)
    {
        workerThread.join();
    }

    for (unsigned int i = 0; i < settings.numThreads; ++i)
    {
        workerThreads.push_back(std::thread(WorkerThread(*this)));
    }

    return true;
}

void threading::ThreadPool::enqueue(Task&& task)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        taskQueue.push(task);
    }

    waitCondition.notify_one();
}

void threading::ThreadPool::enqueue(std::vector<Task>& tasks)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        for (Task task: tasks)
            taskQueue.push(std::move(task));
    }

    waitCondition.notify_all();
}

void threading::ThreadPool::synchronize(Task::Flags flags)
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


