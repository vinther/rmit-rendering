/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef BUFFEREDSHADER_HPP_
#define BUFFEREDSHADER_HPP_


#include "renderer/BufferObject.hpp"
#include "assets/Shader.hpp"

typedef unsigned int GLuint;
typedef int GLint;

namespace renderer
{

class ResourceManager;

class BufferedShader: public BufferObject<assets::Shader>
{
public:
    BufferedShader();
    ~BufferedShader();


    GLuint modelMatrix, viewMatrix, projectionMatrix;

    GLuint program;

    void loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager);

    GLint getUniformLocation(const std::string& name) const;
};

}

#endif /* BUFFEREDSHADER_HPP_ */
