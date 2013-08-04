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
    std::string lowercasePath(path);
//    std::transform(lowercasePath.begin(), lowercasePath.end(), lowercasePath.begin(), ::tolower);

    const SDL_Surface* image = IMG_Load(("assets/" + lowercasePath).c_str());

    if (image)
    {
        surface.reset(image);

        files.clear();
        files.push_back(lowercasePath);

        this->path = lowercasePath;

        ++version;

        return true;
    } else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Texture could not be loaded: \"%s\"", ("assets/" + lowercasePath).c_str());
    }

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
