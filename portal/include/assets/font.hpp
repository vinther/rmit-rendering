#ifndef ASSETS_FONT_HPP_
#define ASSETS_FONT_HPP_

#include "assets/asset.hpp"

#include <memory>

#include <SDL2/SDL_ttf.h>

namespace assets
{

class font: public asset
{
public:
    font(const std::string& name);
    virtual ~font();

    bool loadFromDisk(const std::string& path, unsigned int size);

    size_t reportSize() const;
    void reload();

    TTF_Font* text_font;
};

}

#endif /* ASSETS_FONT_HPP_ */
