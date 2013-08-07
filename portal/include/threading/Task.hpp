/*
 * Task.hpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <functional>

namespace threading
{

class Task
{
public:
    typedef std::function<void()> function_type;

    std::function<void()> f;

    enum Priority
    {
        PRIOTIRY_LOW = 0x1,
        PRIOTIRY_MID = 0x2,
        PRIOTIRY_HIGH = 0x4,
    } priority = Priority::PRIOTIRY_MID;

    enum Flags
    {
        /* Forces task completion before end of frame */
        SYNCHRONIZE_FRAME = 0x1,

        /* Forces task completion before rendering stage */
        SYNCHRONIZE_RENDERING = 0x2,

        /* Causes task to be added back to the queue after completion */
        REENTRANT = 0x4,

        ALL_FLAGS = ~0x0
    } flags = Flags::SYNCHRONIZE_FRAME;

    Task();
    Task(function_type f, Priority = Priority::PRIOTIRY_MID, Flags = Flags::SYNCHRONIZE_FRAME);

    inline void operator()() const
    {
        f();
    }
};

}

#endif /* TASK_HPP_ */
