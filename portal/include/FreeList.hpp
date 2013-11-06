/*
 * FreeList.hpp
 *
 *  Created on: 05/08/2013
 *      Author: svp
 */

#ifndef FREELIST_HPP_
#define FREELIST_HPP_

#include <limits>

template <typename T>
class FreeList
{
    unsigned int insert(T& item);

public:
    std::vector<T> items;
    std::vector<unsigned int> freeList;
};


#endif /* FREELIST_HPP_ */
