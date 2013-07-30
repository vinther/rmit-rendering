/*
 * Utilities.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <memory>

namespace std
{

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#endif /* UTILITIES_HPP_ */
