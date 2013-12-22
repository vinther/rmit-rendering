/*
 * hash.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_UTILITIES_HASH_HPP_
#define CRY_TERRA_UTILITIES_HASH_HPP_

#include <cstdint>
#include <stdexcept>

namespace cry_terra
{

typedef std::int64_t hash_type;

inline hash_type hash(const std::string&, const hash_type hash)
{
    return hash;
}

inline hash_type hash(const std::string&)
{
    throw std::runtime_error("Run-time hash function not implemented yet!");

    return 0;
}

}

#endif /* CRY_TERRA_UTILITIES_HASH_HPP_ */
