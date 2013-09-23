/*
 * Task.cpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#include "threading/task.hpp"

threading::task::task()
    : f([](){})
    , priority(Priority::PRIOTIRY_MID)
    , flags(Flags::SYNCHRONIZE_FRAME)
{
}

threading::task::task(function_type f, Priority priority, Flags flags)
    : f(f)
    , priority(priority)
    , flags(flags)
{
}
