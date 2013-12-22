/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef BUFFEREDSHADER_HPP_
#define BUFFEREDSHADER_HPP_

#include <glm/glm.hpp>


#include "assets/Shader.hpp"
#include "renderer/resources/AssetResource.hpp"

typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace renderer
{

class ResourceManager;

namespace resources
{

class ShaderProgram: public AssetResource<assets::Shader>
{
public:
    ShaderProgram(std::shared_ptr<const asset_type> asset);
    ~ShaderProgram();

    GLuint modelMatrix, viewMatrix, projectionMatrix;
    GLuint program;

    void loadFromAsset(ResourceManager& resourceManager);

    GLint getUniformLocation(const std::string& name) const;

    void setUniform(const std::string& name, const glm::mat4& matrix) const;
    void setUniform(const std::string& name, const glm::vec3& vector) const;
    void setUniform(const std::string& name, const glm::vec4& vector) const;
    void setUniform(const std::string& name, GLint value) const;
    void setUniform(const std::string& name, float value) const;

    GLuint getBlockIndex(const std::string& name) const;

    void setSubroutine(const std::string& name, GLenum type) const;

    void bindUniformBlock(const std::string& name, GLuint location) const;

    void enable() const;
    void disable() const;
};

}

}

#endif /* BUFFEREDSHADER_HPP_ */
