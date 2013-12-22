/*
 * file_extensions.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_SYSTEM_FILE_EXTENSION_HPP_
#define CRY_TERRA_FILE_SYSTEM_FILE_EXTENSION_HPP_


namespace cry_terra
{

namespace file_system
{

enum class file_extension : char
{
    unknown = 0,
    txt = 1,
    obj = 2,
    mtl = 3,
    jpg = 4,
    tga = 5,
    glsl = 6,
};

}

}

#endif /* CRY_TERRA_FILE_SYSTEM_FILE_EXTENSION_HPP_ */
