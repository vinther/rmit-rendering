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

assets::Shader::Shader(const std::string& name)
    : Asset(name, Asset::Type::TYPE_SHADER)
    , vertPath("")
    , fragPath("")
{
    // TODO Auto-generated constructor stub
}

assets::Shader::~Shader()
{
    // TODO Auto-generated destructor stub
}

void assets::Shader::reload()
{
    files.clear();

    loadFromDisk(vertPath, fragPath);
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
bool assets::Shader::loadFromDisk(const std::string& vertPath, const std::string& fragPath)
{
    this->vertPath = vertPath;
    this->fragPath = fragPath;

    this->vertShader = Asset::getStringFromFile("assets/" + vertPath);
    this->fragShader = Asset::getStringFromFile("assets/" + fragPath);

    files.clear();
    files.push_back(vertPath);
    files.push_back(fragPath);

    ++version;

    return true;
}

size_t assets::Shader::reportSize() const
{
    return vertShader.size() + fragShader.size();
}
