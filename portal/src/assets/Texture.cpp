/*
 * Texture.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/Texture.hpp"



Texture::Texture(const std::string& name)
    : Asset(name, Asset::Type::TYPE_TEXTURE)
    , surface()
    , path()
{
}

Texture::~Texture()
{
}

bool Texture::loadFromDisk(const std::string& path)
{
    const SDL_Surface* image = IMG_Load(("assets/" + path).c_str());

    if (image)
    {
        surface.reset(image);

        files.clear();
        files.push_back(path);

        this->path = path;

        return true;
    }

    return false;
}

size_t Texture::reportSize() const
{
    return surface->h * surface->w * surface->format->BytesPerPixel;
}

void Texture::reload()
{
    loadFromDisk(path);
//
//    renderInfo.state |= RenderInfo::DIRTY;
}
