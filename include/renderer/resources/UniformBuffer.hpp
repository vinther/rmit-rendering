/*
 * UniformBuffer.hpp
 *
 *  Created on: 21/08/2013
 *      Author: svp
 */

#ifndef UNIFORMBUFFER_HPP_
#define UNIFORMBUFFER_HPP_

#include <stddef.h>

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

namespace renderer
{
namespace resources
{

class UniformBuffer
{
public:
    UniformBuffer();
    virtual ~UniformBuffer();

    void enable() const;
    void disable() const;

    void bind(GLuint index) const;
    void data(size_t size, const void* data) const;

    GLuint buffer;
};

} /* namespace resources */
} /* namespace renderer */
#endif /* UNIFORMBUFFER_HPP_ */
