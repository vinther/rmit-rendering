/*
 * Texture.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>
#include <memory>

#include <SDL2/SDL_image.h>

#include "assets/Asset.hpp"

class Texture: public Asset
{
public:
    Texture(const std::string& name);
    virtual ~Texture();

    void reload() override;
    size_t reportSize() const override;

    bool loadFromDisk(const std::string& path);

    std::unique_ptr<const SDL_Surface> surface;
    std::string path;
};

#endif /* TEXTURE_HPP_ */
