#ifndef THREADING_WORKER_THREAD_HPP_
#define THREADING_WORKER_THREAD_HPP_

#include <chrono>
#include <memory>

namespace threading
{

class thread_pool;

class worker_thread
{
public:
    worker_thread(thread_pool& threadPool);

    void operator()();

    std::chrono::microseconds workTime;
    std::chrono::high_resolution_clock::time_point timeStarted;
private:
    thread_pool& parent_pool;
};

}

#endif /* THREADING_WORKER_THREAD_HPP_ */
