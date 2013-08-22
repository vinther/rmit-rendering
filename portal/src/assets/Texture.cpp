/*
 * Texture.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/Texture.hpp"

#include <algorithm>

#include <SDL2/SDL_image.h>

assets::Texture::Texture(const std::string& name)
    : Asset(name, Asset::Type::TYPE_TEXTURE)
    , surface()
    , path()
{
}

assets::Texture::~Texture()
{
}

bool assets::Texture::loadFromDisk(const std::string& path)
{
    surface.reset(IMG_Load(("assets/" + path).c_str()));

    if (surface)
    {
        assetFilePaths.clear();
        assetFilePaths.push_back(path);

        ++version;

        this->path = path;

        return true;
    }

	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture could not be loaded: \"%s\"", ("assets/" + path).c_str());

    return false;
}

size_t assets::Texture::reportSize() const
{
    return surface->h * surface->w * surface->format->BytesPerPixel;
}

void assets::Texture::reload()
{
    loadFromDisk(path);
}
