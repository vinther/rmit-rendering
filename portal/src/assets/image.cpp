/*
 * Texture.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/image.hpp"

#include <algorithm>

#include <SDL2/SDL_image.h>

#include "shared/utilities.hpp"

assets::image::image(const std::string& name)
    : asset(name, asset::Type::TYPE_TEXTURE)
    , surface()
    , path()
{
}

assets::image::~image()
{
}

bool assets::image::from_disk(const std::string& path)
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

size_t assets::image::report_size() const
{
    return surface->h * surface->w * surface->format->BytesPerPixel;
}

void assets::image::reload()
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Reloading texture: \"%s\"", identifier.c_str());

    from_disk(path);
}
