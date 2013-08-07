/*
 * Font.hpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#ifndef FONT_HPP_
#define FONT_HPP_

#include "assets/Asset.hpp"

#include <memory>

#include <SDL2/SDL_ttf.h>

namespace assets
{


class Font: public Asset
{
public:
    Font(const std::string& name);
    virtual ~Font();

    bool loadFromDisk(const std::string& path, unsigned int size);

    size_t reportSize() const;
    void reload();

    TTF_Font* font;
};

}

#endif /* FONT_HPP_ */
