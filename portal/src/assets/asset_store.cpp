#include "assets/asset_store.hpp"

#include <iostream>

#include "assets/asset.hpp"
#include "assets/file_watcher.hpp"

#include "shared/utilities.hpp"

assets::asset_store::asset_store()
    : asset_file_watcher()
    , contents()
{
}

assets::asset_store::~asset_store()
{
}

void assets::asset_store::initialize()
{
}

//void assets::asset_store::addToCache(std::string name, std::shared_ptr<asset> asset)
//{
//    cachedAssets[name] = asset;
//
//#ifdef linux
//    for (const auto& path: asset->assetFilePaths)
//    {
//        fileWatcher->addWatchDescriptor(path, asset);
//    }
//#endif
//}
//
//std::shared_ptr<assets::asset> assets::asset_store::fetchFromCache(const std::string& name)
//{
//    return cachedAssets[name];
//}

void assets::asset_store::report_contents() const
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
    for (const auto& item: contents)
    {
        const auto& asset = *(item.second);

        SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS,
                "[%s] \"%s\" (%d bytes)", getTypeName(asset.type).c_str(), asset.identifier.c_str(), asset.report_size());
    }
}
