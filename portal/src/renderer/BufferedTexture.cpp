/*
 * GLBufferedModel.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#include "renderer/BufferedTexture.hpp"

#include <unordered_map>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/ResourceManager.hpp"
#include "Utilities.hpp"

renderer::BufferedTexture::BufferedTexture(std::shared_ptr<const asset_type> asset)
    : BufferObject(asset)
    , tex(0)
{
}

renderer::BufferedTexture::~BufferedTexture()
{
    glDeleteTextures(1, &tex);
}

void renderer::BufferedTexture::loadFromAsset(ResourceManager& resourceManager)
{
    UNUSED(resourceManager);

    if (nullptr == asset->surface)
        throw std::runtime_error("Trying to buffer texture with NULL surface");

    const auto& surface = *(asset->surface);

    if (state & (State::DIRTY | State::PRISTINE))
    {
        glDeleteTextures(1, &tex);

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        //SDL_Log("Bytes per pixel: %d", surface.format->BytesPerPixel);

        if (1 == surface.format->BytesPerPixel)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, surface.w, surface.h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, surface.pixels);
        } else if (3 == surface.format->BytesPerPixel)
        {
            /* Textured are stored in sRGB to have them in linear color space while working on them in shaders! */
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, surface.w, surface.h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface.pixels);
        } else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, surface.w, surface.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface.pixels);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        state = State::READY;
    }
}
