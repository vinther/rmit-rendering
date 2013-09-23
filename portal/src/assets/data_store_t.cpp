/*
 * AssetManager.cpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#include "assets/DataStore.hpp"

#include <iostream>

#include "assets/asset.hpp"
#include "assets/FileWatcher.hpp"

#include "Utilities.hpp"

assets::data_store::data_store()
    : fileWatcher(std::make_unique<file_watcher>())
    , cachedAssets()
{
}

assets::data_store::~data_store()
{
}

void assets::data_store::initialize()
{
}

void assets::data_store::addToCache(std::string name, std::shared_ptr<asset> asset)
{
    cachedAssets[name] = asset;

#ifdef linux
    for (const auto& path: asset->assetFilePaths)
    {
        fileWatcher->addWatchDescriptor(path, asset);
    }
#endif
}

std::shared_ptr<assets::asset> assets::data_store::fetchFromCache(const std::string& name)
{
    return cachedAssets[name];
}

void assets::data_store::reportCacheContents() const
{
    auto getTypeName = [](asset::Type type) -> std::string {
        switch (type)
        {
        case asset::Type::TYPE_NONE: return "None"; break;
        case asset::Type::TYPE_MODEL: return "Model"; break;
        case asset::Type::TYPE_SHADER: return "Shader"; break;
        case asset::Type::TYPE_SOUND: return "Sound"; break;
        case asset::Type::TYPE_TEXTURE: return "Texture"; break;
        case asset::Type::TYPE_MATERIAL: return "Material"; break;
        case asset::Type::TYPE_FONT: return "Font"; break;
        case asset::Type::TYPE_CUSTOM: return "Custom"; break;
        default: return "Unknown"; break;
        }
    };

    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Cached assets: ");
    for (auto it = cachedAssets.cbegin(); it != cachedAssets.cend(); ++it)
    {
        const auto& asset = *(it->second);

        SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS,
                "[%s] \"%s\" (%d bytes)", getTypeName(asset.type).c_str(), asset.name.c_str(), asset.report_size());
    }
}
