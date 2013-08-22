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

#include "Utilities.hpp"

namespace renderer {
namespace resources {

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        SDL_Log("[ERROR] Framebuffer incomplete: Attachment is NOT complete." );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        SDL_Log("[ERROR] Framebuffer incomplete: No image is attached to FBO." );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        SDL_Log("[ERROR] Framebuffer incomplete: Attached images have different dimensions." );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        SDL_Log("[ERROR] Framebuffer incomplete: Color attached images have different internal formats." );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        SDL_Log("[ERROR] Framebuffer incomplete: Draw buffer." );
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        SDL_Log("[ERROR] Framebuffer incomplete: Read buffer." );
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        SDL_Log("[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." );
        return false;

    default:
        SDL_Log("[ERROR] Framebuffer incomplete: Unknown error." );
        return false;
    }
}

FrameBuffer::FrameBuffer()
    : buffer(0)
{
    glGenFramebuffers(1, &buffer);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &buffer);
}

void FrameBuffer::attach(std::unique_ptr<Texture> texture, GLenum attachmentPoint)
{
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texture->buffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    attachments.emplace_back(attachmentPoint, std::move(texture));
}

void FrameBuffer::enable()
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

void FrameBuffer::disable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTextures(
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
