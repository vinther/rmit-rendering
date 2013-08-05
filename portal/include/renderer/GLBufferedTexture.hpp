/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef GLBUFFEREDTEXTURE_HPP_
#define GLBUFFEREDTEXTURE_HPP_

#include "assets/Texture.hpp"

#include "renderer/GLBufferObject.hpp"

class GLResourceManager;

class GLBufferedTexture: public GLBufferObject<assets::Texture>
{
public:
    GLBufferedTexture();
    ~GLBufferedTexture();

    GLuint tex;

    void loadFromAsset(std::shared_ptr<const asset_type>& asset, GLResourceManager& resourceManager);
};

#endif /* GLBUFFEREDTEXTURE_HPP_ */
