/*
 * Shader.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/DiskFile.hpp"

#include <fstream>

#include <SDL2/SDL_log.h>

#include "Utilities.hpp"

assets::DiskFile::DiskFile(const std::string& name)
    : asset(name, asset::Type::TYPE_SHADER)
    , contents()
    , path()
{
    // TODO Auto-generated constructor stub
}

assets::DiskFile::~DiskFile()
{
    // TODO Auto-generated destructor stub
}

void assets::DiskFile::reload()
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Reloading disk file asset: \"%s\"", name.c_str());

    assetFilePaths.clear();

    loadFromDisk(path);
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
bool assets::DiskFile::loadFromDisk(const std::string& path)
{
    this->contents = asset::getStringFromFile("assets/" + path);
    this->path = path;

    assetFilePaths.clear();
    assetFilePaths.push_back(path);

    ++version;

    return true;
}

size_t assets::DiskFile::reportSize() const
{
    return contents.size() * sizeof(std::string::value_type);
}
