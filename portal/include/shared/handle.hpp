/*
 * Handle.hpp
 *
 *  Created on: 23/09/2013
 *      Author: s3443408
 */

#ifndef HANDLE_HPP_
#define HANDLE_HPP_

#include <cstdint>

template <typename T>
struct handle
{
    std::uint32_t index;
    std::uint32_t generation;
};

#endif /* HANDLE_HPP_ */
