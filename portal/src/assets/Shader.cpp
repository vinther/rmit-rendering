/*
 * Shader.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/Shader.hpp"

#include <fstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_log.h>

Shader::Shader(const std::string& name)
    : Asset(name, Asset::Type::TYPE_SHADER)
    , renderInfo({0, 0, 0, 0u})
    , vertPath("")
    , fragPath("")
{
    // TODO Auto-generated constructor stub
}

Shader::~Shader()
{
    // TODO Auto-generated destructor stub
}

void Shader::reload()
{
    files.clear();

    loadFromDisk(vertPath, fragPath);
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
bool Shader::loadFromDisk(const std::string& vertPath, const std::string& fragPath)
{
    this->vertPath = vertPath;
    this->fragPath = fragPath;

    this->vertShader = Asset::getStringFromFile("assets/" + vertPath);
    this->fragShader = Asset::getStringFromFile("assets/" + fragPath);

    files.clear();
    files.push_back(vertPath);
    files.push_back(fragPath);

    glDeleteProgram(renderInfo.program);

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertShaderSrc = this->vertShader.c_str();
    const char *fragShaderSrc = this->fragShader.c_str();

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

        return false;
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

        return false;
    }

    renderInfo.program = glCreateProgram();
    glAttachShader(renderInfo.program, vertShader);
    glAttachShader(renderInfo.program, fragShader);
    glLinkProgram(renderInfo.program);

    glGetProgramiv(renderInfo.program, GL_LINK_STATUS, &result);
    glGetProgramiv(renderInfo.program, GL_INFO_LOG_LENGTH, &logLength);

    if (GL_FALSE == result)
    {
        std::vector<char> programError( (logLength > 1) ? logLength : 1 );
        glGetProgramInfoLog(renderInfo.program, logLength, NULL, programError.data());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLSL shader link error: %s", programError.data());

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return false;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return true;
}

size_t Shader::reportSize() const
{
    return 0;
}
