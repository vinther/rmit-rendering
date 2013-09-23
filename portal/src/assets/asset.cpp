
/*
 * Asset.cpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#include "assets/Asset.hpp"

#include <fstream>
#include <iostream>

#include <SDL2/SDL_log.h>

assets::asset::asset(const std::string& identifier, Type type)
    : identifier(identifier)
    , hash(0)
    , type(type)
    , assetFilePaths(std::vector<std::string>())
{
    std::hash<std::string> hash_fn;
    hash = hash_fn(identifier);
}

assets::asset::~asset()
{
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
std::string assets::asset::getStringFromFile(const std::string& path) const
{
    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if(!fileStream.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader source file does not exist: \"%s\"", path.c_str());
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
