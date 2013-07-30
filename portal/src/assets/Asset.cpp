
/*
 * Asset.cpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#include "assets/Asset.hpp"

#include <SDL2/SDL_log.h>

Asset::Asset(const std::string& name, Type type)
    : name(name)
    , type(type)
{
    std::hash<std::string> hash_fn;
    hash = hash_fn(name);
}

Asset::~Asset()
{
}

void Asset::reload()
{
    SDL_Log("Reloading \"%s\"", name.c_str());
}
