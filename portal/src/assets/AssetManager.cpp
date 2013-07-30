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

std::shared_ptr<Asset> AssetManager::fetchFromCache(const std::string& name)
{
    return cachedAssets[name];
}

template <class T>
std::shared_ptr<T> AssetManager::fetchFromDisk(const std::string& name)
{
    const auto cachedAsset = cachedAssets.find(name);
    const bool cached = std::end(cachedAssets) != cachedAsset;

    auto asset = cached ? std::static_pointer_cast<T>(cachedAsset->second) : std::make_shared<T>(name);

    if (asset->loadFromDisk(name, *this))
        cachedAssets[name] = asset;
    else
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Asset file could not loaded from disk: \"%s\"", name.c_str());

    return asset;
}

template std::shared_ptr<Model> AssetManager::fetchFromDisk<Model>(const std::string& name);
template std::shared_ptr<Material> AssetManager::fetchFromDisk<Material>(const std::string& name);

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

void AssetManager::initialize()
{
#ifdef linux
    inotifyFileDescriptor = inotify_init1(IN_NONBLOCK);

    if (0 > inotifyFileDescriptor)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize inotify");
#endif
}



void AssetManager::checkForFileChanges()
{
#ifdef linux
    ssize_t length = read(inotifyFileDescriptor, inotifyBuffer.data(), inotifyBuffer.size());

    if (0 > length && 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Error while reading from inotify (length < 0) %d, %d", length, inotifyBuffer.size());
        perror("File Read Error");
        return;
    }

    ssize_t i = 0;
    while (i < length)
    {
        inotify_event *event = (inotify_event*) &inotifyBuffer[i];
        if (event->len)
        {
            const auto& assetName = inotifyWatchDescriptors.at(event->wd);
            std::cout << assetName << std::endl;
            if (event->mask & IN_MODIFY)
            {
                cachedAssets[assetName]->loadFromDisk(assetName, *this);
            }
        }

        i += sizeof(inotify_event) + event->len;
    }
#endif
}

#ifdef linux



void AssetManager::addWatchDescriptor(const std::string& name)
{
    int watchDescriptor = inotify_add_watch(inotifyFileDescriptor, ("assets/" + name).c_str(), IN_MODIFY);

    if (0 > watchDescriptor)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to add inotify watch for asset \"%s\"", name.c_str());
        return;
    }

    inotifyWatchDescriptors[watchDescriptor] = name;
}
#endif
