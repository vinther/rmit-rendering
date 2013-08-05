/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef GLBUFFEREDSHADER_HPP_
#define GLBUFFEREDSHADER_HPP_


#include "renderer/GLBufferObject.hpp"
#include "assets/Shader.hpp"

typedef unsigned int GLuint;

class GLResourceManager;

class GLBufferedShader: public GLBufferObject<assets::Shader>
{
public:
    GLBufferedShader();
    ~GLBufferedShader();


    GLuint modelMatrix, viewMatrix, projectionMatrix;

    GLuint program;

    void loadFromAsset(std::shared_ptr<const asset_type>& asset, GLResourceManager& resourceManager);
};

#endif /* GLBUFFEREDSHADER_HPP_ */
