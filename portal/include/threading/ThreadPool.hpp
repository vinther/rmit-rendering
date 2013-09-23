/*
 * ThreadPool.hpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <atomic>
#include <condition_variable>

#include "threading/Task.hpp"
#include "threading/WorkerThread.hpp"

namespace threading
{

class thread_pool
{
public:
    struct Settings
    {
        unsigned int numThreads;
    } settings;

    thread_pool();
    virtual ~thread_pool();

    bool initialize();
    void enqueue(Task&& task);
    void enqueue(std::vector<Task>& tasks);
    void synchronize(Task::Flags flags = Task::Flags::ALL_FLAGS);

    unsigned int getNumTasks() const
    {
        return taskQueue.size();
    }

    const std::vector<std::shared_ptr<worker_thread>>& getWorkers() const
    {
        return workerThreadObjects;
    }
private:
    friend class worker_thread;

    void registerWorker(std::shared_ptr<worker_thread> worker)
    {
        std::unique_lock<std::mutex> lock(registerMutex);

        workerThreadObjects.push_back(worker);
    }

    std::mutex registerMutex;

    std::vector<std::thread> workerThreads;
    std::vector<std::shared_ptr<worker_thread>> workerThreadObjects;

    struct TaskCompare
    {
        inline bool operator()(const Task& a, const Task& b) const
        {
            return a.priority < b.priority;
        }
    };

    std::priority_queue<Task, std::vector<Task>, TaskCompare> taskQueue;

    std::mutex queueMutex;
    std::mutex syncMutex;

    std::condition_variable waitCondition;
    std::condition_variable syncCondition;

    std::atomic_bool isSynchronizing;

    bool stop;
};

} /* namespace threading */

#endif /* THREADPOOL_HPP_ */
