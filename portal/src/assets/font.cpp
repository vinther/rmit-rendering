#include "assets/font.hpp"

assets::font::font(const std::string& name)
    : asset(name, asset::Type::TYPE_FONT)
    , text_font(nullptr)
{
}

assets::font::~font()
{
}

bool assets::font::loadFromDisk(const std::string& path, unsigned int size)
{
    text_font = TTF_OpenFont(("assets/" + path).c_str(), size);

    return (font);
}

size_t assets::font::reportSize() const
{
    return 0;
}

void assets::font::reload()
{
}
