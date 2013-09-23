/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/resources/ShaderProgram.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>

#include "renderer/resources/Material.hpp"
#include "assets/DiskFile.hpp"

#include "Utilities.hpp"

namespace renderer
{

namespace resources
{


ShaderProgram::ShaderProgram()
	: modelMatrix(0), viewMatrix(0), projectionMatrix(0)
    , program(0)
{

}

ShaderProgram::~ShaderProgram()
{
    // TODO Auto-generated destructor stub
}

inline GLint getUniformLocationImpl(GLuint program, const std::string& name)
{
    const GLint location = glGetUniformLocation(program, name.c_str());

    return location;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
    return getUniformLocationImpl(program, name.c_str());
}

void ShaderProgram::enable() const
{
    glUseProgram(program);
}

void ShaderProgram::disable() const
{
    glUseProgram(0);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniform(const std::string& name, GLint value) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string& name, float value) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vector) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& vector) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform4fv(location, 1, glm::value_ptr(vector));
}

GLuint ShaderProgram::getBlockIndex(const std::string& name) const
{
    return glGetUniformBlockIndex(program, name.c_str());
}

void ShaderProgram::bindUniformBlock(const std::string& name, GLuint location) const
{
    glUniformBlockBinding(program, getBlockIndex(name), location);
}

void ShaderProgram::setSubroutine(const std::string& name, GLenum type) const
{
    GLuint index = glGetSubroutineIndex(program, type, name.c_str());
    glUniformSubroutinesuiv(type, 1, &index);
}

void ShaderProgram::attachShader(ShaderType type, std::shared_ptr<assets::DiskFile> asset)
{
    assets[type] = asset;

    if (program)
        glDeleteProgram(program);

    program = glCreateProgram();

    for (const auto& dims: {
            std::make_pair(SHADER_TYPE_VERTEX, GL_VERTEX_SHADER),
            std::make_pair(SHADER_TYPE_FRAGMENT, GL_FRAGMENT_SHADER),
            std::make_pair(SHADER_TYPE_GEOMETRY, GL_GEOMETRY_SHADER)})
    {
        GLuint shader = glCreateShader(dims.second);
        const char* src = assets[dims.first]->contents.c_str();

        GLint result = GL_FALSE;
        int logLength;

        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        if (GL_FALSE == result)
        {
            std::vector<char> vertShaderError(logLength);
            glGetShaderInfoLog(shader, logLength, NULL, vertShaderError.data());
            SDL_LogError(client::PORTAL_LOG_CATEGORY_RENDERER, "Shader compile error: %s", vertShaderError.data());
        }

        glAttachShader(program, shader);

        glDeleteShader(shader);
    }

    glLinkProgram(program);
}

}

}


