/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/BufferedShader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/scene.h>

#include "renderer/ResourceManager.hpp"
#include "assets/Shader.hpp"

#include "Utilities.hpp"

renderer::BufferedShader::BufferedShader()
    : modelMatrix(0), viewMatrix(0), projectionMatrix(0)
    , program(0)
{

}

renderer::BufferedShader::~BufferedShader()
{
    // TODO Auto-generated destructor stub
}

void renderer::BufferedShader::loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager)
{
    UNUSED(resourceManager);
    glDeleteProgram(program);

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

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

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
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

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
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

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

GLint renderer::BufferedShader::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(program, name.c_str());
}
