/*
 * file_descriptor.hpp
 *
 *  Created on: 28/10/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_DESCRIPTOR_HPP_
#define CRY_TERRA_FILE_DESCRIPTOR_HPP_

#include <cstdint>

#include <cry_terra/utilities/handle.hpp>

namespace cry_terra
{

namespace file_system
{

struct packaged_file_descriptor
{
    std::int64_t path_hash;
    std::uint32_t adler32;
    std::uint32_t compression;
    size_t begin;
    size_t size;
    size_t original_size;
};

typedef utilities::handle<packaged_file_descriptor> file_descriptor_handle;

}

}

#endif /* CRY_TERRA_FILE_DESCRIPTOR_HPP_ */
