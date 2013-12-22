/*
 * Handle.hpp
 *
 *  Created on: 30/08/2013
 *      Author: svp
 */

#ifndef HANDLE_HPP_
#define HANDLE_HPP_

#include <cstdint>

template <typename T>
struct Handle
{
    std::int32_t index;
    std::int32_t generation;
};

#endif /* HANDLE_HPP_ */
