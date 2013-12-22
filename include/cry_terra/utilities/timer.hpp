/*
 * timer.hpp
 *
 *  Created on: 09/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_UTILITIES_TIMER_HPP_
#define CRY_TERRA_UTILITIES_TIMER_HPP_

#include <chrono>

#include <cry_terra/log/log.hpp>

namespace cry_terra
{

namespace utilities
{

class timer
{
public:
    typedef std::function<void(int64_t)> timeout_function_type;

    timer(timeout_function_type on_timeout)
        : on_timeout(on_timeout)
    {
        begin = std::chrono::high_resolution_clock::now();
    }

    timer()
        : on_timeout()
    {
        begin = std::chrono::high_resolution_clock::now();
    }

    ~timer()
    {
        end = std::chrono::high_resolution_clock::now();

        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        if (on_timeout)
            on_timeout(duration);
        else
            log::log_error(log::category::file_system, "Timer execution in %d microseconds", duration);
    }

private:
    std::chrono::high_resolution_clock::time_point begin, end;

    timeout_function_type on_timeout;
};


}

}

#endif /* CRY_TERRA_UTILITIES_TIMER_HPP_ */
