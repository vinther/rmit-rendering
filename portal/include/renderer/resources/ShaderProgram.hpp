/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <array>

#include <glm/glm.hpp>

#include "assets/DiskFile.hpp"
#include "renderer/resources/AssetResource.hpp"

typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace renderer
{

class ResourceManager;

namespace resources
{

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint modelMatrix, viewMatrix, projectionMatrix;
    GLuint program;

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

    enum ShaderType
    {
        SHADER_TYPE_VERTEX = 0,
        SHADER_TYPE_FRAGMENT = 1,
        SHADER_TYPE_GEOMETRY = 2,

        SHADER_TYPE_SIZE
    };

    void attachShader(ShaderType type, std::shared_ptr<assets::DiskFile> asset);

    std::array<std::shared_ptr<assets::DiskFile>, SHADER_TYPE_SIZE> assets;
};

}

}

#endif /* SHADER_PROGRAM_HPP */
