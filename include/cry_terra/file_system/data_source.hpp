/*
 * data_source.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_SYSTEM_DATA_SOURCE_HPP_
#define CRY_TERRA_FILE_SYSTEM_DATA_SOURCE_HPP_

#include <cry_terra/file_system/file_extension.hpp>
#include <cry_terra/file_system/file_stream.hpp>
#include <cry_terra/utilities/hash.hpp>

namespace cry_terra
{

namespace file_system
{

class data_source
{
public:
    virtual ~data_source() {}
    virtual file_stream open_file(const hash_type) = 0;
};

}

}

#endif /* CRY_TERRA_FILE_SYSTEM_DATA_SOURCE_HPP_ */
