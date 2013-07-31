/*
 * AssetManager.cpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#include "assets/AssetManager.hpp"

#include <iostream>

#include <SDL2/SDL_log.h>

#include "assets/Asset.hpp"
#include "assets/Model.hpp"
#include "assets/Material.hpp"
#include "assets/FileWatcher.hpp"

#include "Utilities.hpp"

void AssetManager::initialize()
{
}

AssetManager::AssetManager()
    : fileWatcher(std::make_unique<FileWatcher>())
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::addToCache(std::string name, std::shared_ptr<Asset> asset)
{
    cachedAssets[name] = asset;

    for (const auto& path: asset->files)
    {
        fileWatcher->addWatchDescriptor(path, asset);
    }
}

std::shared_ptr<Asset> AssetManager::fetchFromCache(const std::string& name)
{
    return cachedAssets[name];
}

void AssetManager::reportCacheContents() const
{
    auto getTypeName = [](Asset::Type type) -> std::string {
        switch (type)
        {
        case Asset::Type::TYPE_NONE: return "None"; break;
        case Asset::Type::TYPE_MODEL: return "Model"; break;
        case Asset::Type::TYPE_SHADER: return "Shader"; break;
        case Asset::Type::TYPE_SOUND: return "Sound"; break;
        case Asset::Type::TYPE_CUSTOM: return "Custom"; break;
        default: return "Unknown"; break;
        }
    };

    SDL_Log("Cached assets: ");
    for (auto it = cachedAssets.cbegin(); it != cachedAssets.cend(); ++it)
    {
        const auto& asset = *(it->second);

        SDL_Log("[%s] \"%s\" (%d bytes)", getTypeName(asset.type).c_str(), asset.name.c_str(), asset.reportSize());
    }
}
