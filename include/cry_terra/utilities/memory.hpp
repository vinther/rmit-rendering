/*
 * memory.hpp
 *
 *  Created on: 09/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_UTILITIES_MEMORY_HPP_
#define CRY_TERRA_UTILITIES_MEMORY_HPP_

#include <memory>

namespace cry_terra
{

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
std::shared_ptr<T> make_shared(Args&&... args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#endif /* CRY_TERRA_UTILITIES_MEMORY_HPP_ */
