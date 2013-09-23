/*
 * FrameBuffer.cpp
 *
 *  Created on: 19/08/2013
 *      Author: svp
 */

#include "renderer/resources/FrameBuffer.hpp"

#include <algorithm>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/resources/Texture.hpp"

#include "shared/utilities.hpp"

namespace renderer {
namespace resources {

frame_buffer::frame_buffer()
    : buffer(0)
{
    glGenFramebuffers(1, &buffer);
}

frame_buffer::~frame_buffer()
{
	glDeleteFramebuffers(1, &buffer);
}

void frame_buffer::attach(std::unique_ptr<Texture> texture, GLenum attachmentPoint)
{
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texture->buffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    attachments.emplace_back(attachmentPoint, std::move(texture));
}

void frame_buffer::enableAll()
{
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    std::vector<GLenum> v;
    v.reserve(attachments.size());

    for (const auto& attachment: attachments)
    {
        if (attachment.first >= GL_COLOR_ATTACHMENT0 && attachment.first <= GL_COLOR_ATTACHMENT15)
            v.push_back(attachment.first);
    }

    std::sort(std::begin(v), std::end(v));

    glDrawBuffers(v.size(), v.data());
}

void frame_buffer::enable(const std::initializer_list<GLenum> attachments)
{
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glDrawBuffers(attachments.size(), attachments.begin());
}

void frame_buffer::disable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame_buffer::bindTextures(
        const std::initializer_list<binding_type>& bindings)
{
    for (const auto& binding: bindings)
    {
        const auto& result = std::find_if(std::begin(attachments), std::end(attachments),
                [&](const attachment_type& attachment) { return binding.first == attachment.first; });

        if (std::end(attachments) == result)
            throw std::runtime_error("Attachment point does not exist in frame buffer");

        glActiveTexture(binding.second);
        glBindTexture(GL_TEXTURE_2D, result->second->buffer);
    }

}

} /* namespace resources */
} /* namespace renderer */
