/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef BUFFEREDSHADER_HPP_
#define BUFFEREDSHADER_HPP_

#include <glm/glm.hpp>

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
    BufferedShader(std::shared_ptr<const asset_type> asset);
    ~BufferedShader();

    GLuint modelMatrix, viewMatrix, projectionMatrix;
    GLuint program;

    void loadFromAsset(ResourceManager& resourceManager);

    GLint getUniformLocation(const std::string& name) const;

    void setUniform(const std::string& name, const glm::mat4& matrix) const;
    void setUniform(const std::string& name, GLint value) const;
    void setUniform(const std::string& name, float value) const;

    void enable() const;
    void disable() const;
};

}

#endif /* BUFFEREDSHADER_HPP_ */
