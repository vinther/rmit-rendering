#include "assets/file.hpp"

#include <fstream>

#include <SDL2/SDL_log.h>

#include "shared/utilities.hpp"

assets::file::file(const std::string& name)
    : asset(name, asset::Type::TYPE_TEXT_FILE)
    , contents()
    , path()
{
}

assets::file::~file()
{
}

void assets::file::reload()
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Reloading disk file asset: \"%s\"", identifier.c_str());

    assetFilePaths.clear();

    loadFromDisk(path);
}

/* http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */
bool assets::file::loadFromDisk(const std::string& path)
{
    this->contents = asset::getStringFromFile("assets/" + path);
    this->path = path;

    assetFilePaths.clear();
    assetFilePaths.push_back(path);

    return true;
}

size_t assets::file::reportSize() const
{
    return contents.size() * sizeof(std::string::value_type);
}
