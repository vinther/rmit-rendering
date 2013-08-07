/*
 * WorkerThread.hpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#ifndef WORKERTHREAD_HPP_
#define WORKERTHREAD_HPP_

#include <chrono>
#include <memory>

namespace threading
{

class ThreadPool;

class WorkerThread
{
public:
    WorkerThread(ThreadPool& threadPool);

    void operator()();

    std::chrono::microseconds workTime;
    std::chrono::high_resolution_clock::time_point timeStarted;
private:
    ThreadPool& threadPool;
};

}

#endif /* WORKERTHREAD_HPP_ */
