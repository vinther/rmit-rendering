/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef BUFFEREDTEXTURE_HPP_
#define BUFFEREDTEXTURE_HPP_

#include "assets/Texture.hpp"

#include "renderer/BufferObject.hpp"


namespace renderer
{

class ResourceManager;

class BufferedTexture: public BufferObject<assets::Texture>
{
public:
    BufferedTexture(std::shared_ptr<const asset_type> asset);
    ~BufferedTexture();

    GLuint tex;

    void loadFromAsset(ResourceManager& resourceManager);
};

}

#endif /* BUFFEREDTEXTURE_HPP_ */
