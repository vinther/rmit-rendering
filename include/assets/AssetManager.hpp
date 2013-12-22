/*
 * AssetManager.hpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_

#include <memory>
#include <array>
#include <string>
#include <unordered_map>
#include <type_traits>

#include <SDL2/SDL_log.h>

#include "assets/Asset.hpp"
#include "assets/Model.hpp"

namespace assets
{
class FileWatcher;

//namespace detail
//{
//template<class T>
//    using EnableIfBase = typename std::enable_if<std::is_base_of<Asset, T>::value>::type;
//}

class AssetManager
{
public:
    AssetManager();
    virtual ~AssetManager();

    template <  class T/*, class = detail::EnableIfBase<T>*/>
    std::shared_ptr<T> get(const std::string& name);

    template <class T/*, class = detail::EnableIfBase<T>*/, typename... Args>
    std::shared_ptr<T> create(const std::string& name, Args... args);

    template <class T/*, class = detail::EnableIfBase<T>*/, typename... Args>
    std::shared_ptr<T> getOrCreate(const std::string& name, Args... args);

    void initialize();
    void reportCacheContents() const;

    std::unique_ptr<FileWatcher> fileWatcher;
private:
    void addToCache(std::string name, std::shared_ptr<Asset> asset);

    std::shared_ptr<Asset> fetchFromCache(const std::string& name);
    std::unordered_map<std::string, std::shared_ptr<Asset>> cachedAssets;
};

template <class T/*, class Enable*/>
inline std::shared_ptr<T> AssetManager::get(const std::string& name)
{
    const auto& cachedAsset = cachedAssets.find(name);

    if (std::end(cachedAssets) == cachedAsset)
    {
        //SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        //        "Asset not found in cache: \"%s\" (create first or use getOrCreate!)", name.c_str());
        return std::make_shared<T>(name);
    } else
        return std::static_pointer_cast<T>(fetchFromCache(name));

    return std::make_shared<T>(name);
}

template<class T/*, class Enable*/, typename... Args>
inline std::shared_ptr<T> AssetManager::create(const std::string& name, Args... args)
{
    auto asset = std::make_shared<T>(name);
    if (!asset->loadFromDisk(std::forward<Args>(args)...))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Asset could not be loaded: \"%s\"", name.c_str());
    } else
    {
        addToCache(name, asset);
    }

    return asset;
}

template<class T/*, class Enable*/, typename... Args>
inline std::shared_ptr<T> AssetManager::getOrCreate(const std::string& name, Args... args)
{
    const auto& cachedAsset = cachedAssets.find(name);

    if (std::end(cachedAssets) == cachedAsset)
    {
        return create<T>(name, std::forward<Args>(args)...);
    } else
        return std::static_pointer_cast<T>(fetchFromCache(name));

    return std::make_shared<T>(name);
}

}

#endif /* ASSETMANAGER_HPP_ */
