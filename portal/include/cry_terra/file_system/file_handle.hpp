/*
 * file_handle.hpp
 *
 *  Created on: 27/10/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_HANDLE_HPP_
#define CRY_TERRA_FILE_HANDLE_HPP_

#include <cry_terra/utilities/handle.hpp>
#include <cry_terra/file_system/file_descriptor.hpp>

namespace cry_terra
{

namespace file_system
{

typedef utilities::handle<file_descriptor> file_descriptor_handle;

}

}

#endif /* CRY_TERRA_FILE_HANDLE_HPP_ */
