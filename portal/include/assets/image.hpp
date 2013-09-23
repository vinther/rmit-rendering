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

#include "assets/Asset.hpp"

class SDL_Surface;

typedef unsigned int GLuint;

namespace assets
{

class image: public asset
{
public:
    image(const std::string& name);
    virtual ~image();

    void reload();
    size_t report_size() const;

    bool from_disk(const std::string& path);

    std::unique_ptr<const SDL_Surface> surface;
    std::string path;
};

}

#endif /* TEXTURE_HPP_ */
