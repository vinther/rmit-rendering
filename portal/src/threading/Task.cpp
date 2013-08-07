/*
 * Task.cpp
 *
 *  Created on: 07/08/2013
 *      Author: svp
 */

#include "threading/Task.hpp"

threading::Task::Task()
    : Task([](){})
{
}

threading::Task::Task(function_type f, Priority priority, Flags flags)
    : f(f)
    , priority(priority)
    , flags(flags)
{
}
