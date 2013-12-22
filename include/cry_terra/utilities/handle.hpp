/*
 * handle.hpp
 *
 *  Created on: 28/10/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_HANDLE_HPP_
#define CRY_TERRA_HANDLE_HPP_

#include <cstdint>

namespace cry_terra
{

namespace utilities
{

template <typename T>
struct handle
{
    std::uint32_t index;
    std::uint32_t generation;
};

}

}

#endif /* CRY_TERRA_HANDLE_HPP_ */
