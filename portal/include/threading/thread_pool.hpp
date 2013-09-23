#ifndef THREADING_THREAD_POOL_HPP_
#define THREADING_THREAD_POOL_HPP_

#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <atomic>
#include <condition_variable>

#include "threading/task.hpp"
#include "threading/worker_thread.hpp"

namespace threading
{

class thread_pool
{
public:
    struct settings_t
    {
        unsigned int num_threads;
    } settings;

    thread_pool();
    virtual ~thread_pool();

    bool initialize();
    void enqueue(task&& task);
    void enqueue(std::vector<task>& tasks);
    void synchronize(task::Flags flags = task::Flags::ALL_FLAGS);

    unsigned int num_tasks() const
    {
        return taskQueue.size();
    }

    const std::vector<std::shared_ptr<worker_thread>>& workers() const
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
        inline bool operator()(const task& a, const task& b) const
        {
            return a.priority < b.priority;
        }
    };

    std::priority_queue<task, std::vector<task>, TaskCompare> taskQueue;

    std::mutex queueMutex;
    std::mutex syncMutex;

    std::condition_variable waitCondition;
    std::condition_variable syncCondition;

    std::atomic_bool isSynchronizing;

    bool stop;
};

} /* namespace threading */

#endif /* THREADING_THREAD_POOL_HPP_ */
