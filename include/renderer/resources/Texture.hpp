/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef RENDERER_RESOURCES_TEXTURE_HPP_
#define RENDERER_RESOURCES_TEXTURE_HPP_

#include "assets/Texture.hpp"

#include "renderer/resources/AssetResource.hpp"

typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace renderer
{

class ResourceManager;

namespace resources
{

class Texture: public AssetResource<assets::Texture>
{
public:
    Texture(std::shared_ptr<const asset_type> asset);
    ~Texture();

    enum MipMapMode
    {
        NO_MIPMAP = 0,
        MIPMAP = 1,
    };

    GLuint buffer;

    void loadFromAsset(ResourceManager& resourceManager);

    void createFromData(void* data, unsigned int width, unsigned int height, unsigned int bytesPerPixel);
    void createBlank(GLint internalFormat, unsigned int width, unsigned int height, GLenum format, GLenum type, MipMapMode mipMapMode);
};

}

}

#endif /* BUFFEREDTEXTURE_HPP_ */
