/*
 * GLBufferedModel.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#include "renderer/resources/Texture.hpp"

#include <unordered_map>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/ResourceManager.hpp"
#include "Utilities.hpp"

renderer::resources::Texture::Texture(std::shared_ptr<const asset_type> asset)
    : AssetResource(asset)
    , buffer(0)
{
}

renderer::resources::Texture::~Texture()
{
    glDeleteTextures(1, &buffer);
}

void renderer::resources::Texture::loadFromAsset(storage_backend& resourceManager)
{
    UNUSED(resourceManager);

    if (nullptr == asset->surface)
        throw std::runtime_error("Trying to buffer texture with NULL surface");

    const auto& surface = *(asset->surface);

    if ((state & (State::DIRTY | State::PRISTINE)) && surface.pixels)
    {
        createFromData(surface.pixels, surface.w, surface.h, surface.format->BytesPerPixel);
    }
}

void renderer::resources::Texture::createFromData(
		void* data,
		unsigned int width, unsigned int height,
		unsigned int bytesPerPixel)
{
    if (buffer)
        glDeleteTextures(1, &buffer);

    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);

    //SDL_Log("Bytes per pixel for %s: %d", asset->name.c_str(), surface.format->BytesPerPixel);

    switch (bytesPerPixel)
    {
    case 1:
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
    	break;
    case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		break;
    case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		break;
    default:
    	throw new std::runtime_error("Creating texture width other than 1, 3 or 4 bytes per pixel");
    	break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    state = State::READY;
}

void renderer::resources::Texture::createBlank(GLint internalFormat,
        unsigned int width, unsigned int height, GLenum format, GLenum type, MipMapMode mipMapMode)
{
    if (buffer)
        glDeleteTextures(1, &buffer);

    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);

    if (mipMapMode == MipMapMode::MIPMAP)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }


    glBindTexture(GL_TEXTURE_2D, 0);

    state = State::READY;
}
