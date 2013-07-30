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
#include <unordered_map>
#include <type_traits>

#ifdef linux
#include <unistd.h>
#include <sys/inotify.h>
#endif

#include <assimp/Importer.hpp>

class Asset;

class AssetManager
{
public:
    template <class T>
    std::weak_ptr<T> get(const std::string& name, bool reload = false);

    void initialize();
    void checkForFileChanges();
    void reportCacheContents() const;

private:
    template <class T>
    std::shared_ptr<T> fetchFromCache(const std::string& name);

    template <class T>
    std::shared_ptr<T> fetchFromDisk(const std::string& name);

    std::unordered_map<std::string, std::shared_ptr<Asset>> cachedAssets;

#ifdef linux
    int inotifyFileDescriptor;
    std::array<char, 1024 * sizeof(inotify_event)> inotifyBuffer;
    std::unordered_map<int, std::string> inotifyWatchDescriptors;

    void addWatchDescriptor(const std::string& name);
    void removeWatchDescriptor(const std::string& name);
#endif
};

template<class T>
inline std::weak_ptr<T> AssetManager::get(const std::string& name, bool reload)
{
    const auto& cachedAsset = cachedAssets.find(name);

    bool cached = !(std::end(cachedAssets) == cachedAsset || reload);

    return std::weak_ptr<T>(cached ? fetchFromCache<T>(name) : fetchFromDisk<T>(name));
}



#endif /* ASSETMANAGER_HPP_ */
