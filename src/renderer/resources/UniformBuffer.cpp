/*
 * UniformBuffer.cpp
 *
 *  Created on: 21/08/2013
 *      Author: svp
 */

#include "renderer/resources/UniformBuffer.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

namespace renderer
{
namespace resources
{

UniformBuffer::UniformBuffer()
{
    glGenBuffers(1, &buffer);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &buffer);
}


void UniformBuffer::enable() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
}

void UniformBuffer::disable() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bind(GLuint index) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);
}

void UniformBuffer::data(size_t size, const void* data) const
{
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

} /* namespace resources */
} /* namespace renderer */


