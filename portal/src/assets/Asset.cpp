
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

assets::Asset::Asset(const std::string& name, Type type)
    : name(name)
    , hash(0)
    , type(type)
    , assetFilePaths(std::vector<std::string>())
    , version(0)
{
    std::hash<std::string> hash_fn;
    hash = hash_fn(name);
}

assets::Asset::~Asset()
{
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
std::string assets::Asset::getStringFromFile(const std::string& path) const
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
