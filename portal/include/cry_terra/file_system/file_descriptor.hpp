/*
 * file_descriptor.hpp
 *
 *  Created on: 28/10/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_DESCRIPTOR_HPP_
#define CRY_TERRA_FILE_DESCRIPTOR_HPP_

#include <cstdint>
#include <boost/filesystem/path.hpp>

#include <cry_terra/utilities/handle.hpp>

namespace cry_terra
{

namespace file_system
{

struct file_descriptor
{
    boost::filesystem::path path;
    size_t begin;
    size_t size;
    size_t original_size;
    uint32_t adler32;
    uint32_t compression;

};

typedef utilities::handle<file_descriptor> file_descriptor_handle;

}

}

#endif /* CRY_TERRA_FILE_DESCRIPTOR_HPP_ */
