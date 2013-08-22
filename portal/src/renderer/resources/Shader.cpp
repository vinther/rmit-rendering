/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/resources/Shader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>

#include "renderer/resources/Material.hpp"
#include "assets/Shader.hpp"

#include "Utilities.hpp"

namespace renderer
{

namespace resources
{


Shader::Shader(std::shared_ptr<const asset_type> asset)
    : AssetResource(asset)
	, modelMatrix(0), viewMatrix(0), projectionMatrix(0)
    , program(0)
{

}

Shader::~Shader()
{
    // TODO Auto-generated destructor stub
}

void Shader::loadFromAsset(ResourceManager& resourceManager)
{
    UNUSED(resourceManager);

    GLint oldProgram = program;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    GLenum errCode;
    const GLubyte *errString;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
       fprintf (stderr, "OpenGL Error: %s\n", errString);
    }

    const char *vertShaderSrc = asset->vertShader.c_str();
    const char *fragShaderSrc = asset->fragShader.c_str();

    GLint result = GL_FALSE;
    int logLength;

    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

    if (GL_FALSE == result)
    {
        std::vector<char> vertShaderError(logLength);
        glGetShaderInfoLog(vertShader, logLength, NULL, vertShaderError.data());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Vertex shader error: %s", vertShaderError.data());
    }

    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

    if (GL_FALSE == result)
    {
        std::vector<char> fragShaderError(logLength);
        glGetShaderInfoLog(fragShader, logLength, NULL, fragShaderError.data());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Fragment shader error: %s", fragShaderError.data());
    }

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    if (GL_FALSE == result)
    {
        std::vector<char> programError( (logLength > 1) ? logLength : 1 );
        glGetProgramInfoLog(program, logLength, NULL, programError.data());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLSL shader link error: %s", programError.data());
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glDeleteProgram(oldProgram);

    currentVersion = asset->version;
}

inline GLint getUniformLocationImpl(GLuint program, const std::string& name)
{
    const GLint location = glGetUniformLocation(program, name.c_str());

    return location;
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    return getUniformLocationImpl(program, name.c_str());
}

void Shader::enable() const
{
    glUseProgram(program);
}

void Shader::disable() const
{
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string& name, GLint value) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, float value) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& vector) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, const glm::vec4& vector) const
{
    const auto location = getUniformLocationImpl(program, name);

    glUniform4fv(location, 1, glm::value_ptr(vector));
}

GLuint Shader::getBlockIndex(const std::string& name) const
{
    return glGetUniformBlockIndex(program, name.c_str());
}

void Shader::bindUniformBlock(const std::string& name, GLuint location) const
{
    glUniformBlockBinding(program, getBlockIndex(name), location);
}

void Shader::setSubroutine(const std::string& name, GLenum type) const
{
    GLuint index = glGetSubroutineIndex(program, type, name.c_str());
    glUniformSubroutinesuiv(type, 1, &index);
}

}

}


