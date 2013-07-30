/*
 * AssetManager.cpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#include "assets/AssetManager.hpp"

#include <iostream>

#include <SDL2/SDL_log.h>

template<>
std::weak_ptr<Asset> AssetManager::get<Asset>(const std::string& name)
{
    auto cachedAsset = cachedAssets.find(name);
    if (std::end(cachedAssets) == cachedAsset)
    {
        const aiScene* scene = importer.ReadFile("assets/" + name,
                aiProcess_CalcTangentSpace       |
                aiProcess_Triangulate            |
                aiProcess_JoinIdenticalVertices  |
                aiProcess_SortByPType);

        if (scene)
        {
            cachedAssets[name] = std::make_shared<Asset>(scene);
            return std::weak_ptr<Asset>(cachedAssets[name]);
        } else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Model file could not be found: \"%s\"", name.c_str());
            return std::weak_ptr<Asset>();
        }
    } else
    {
        return std::weak_ptr<Asset>(cachedAsset->second);
    }
}







