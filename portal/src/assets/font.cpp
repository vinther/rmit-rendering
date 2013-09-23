/*
 * Font.cpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#include "assets/Font.hpp"



assets::Font::Font(const std::string& name)
    : asset(name, asset::Type::TYPE_FONT)
    , font(nullptr)
{
}

assets::Font::~Font()
{
}

bool assets::Font::loadFromDisk(const std::string& path, unsigned int size)
{
    font = TTF_OpenFont(("assets/" + path).c_str(), size);

    return (font);
}

size_t assets::Font::reportSize() const
{
    return 0;
}

void assets::Font::reload()
{
}
