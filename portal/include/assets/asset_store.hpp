#ifndef UTILITIES_DATA_STORE_HPP
#define UTILITIES_DATA_STORE_HPP

#include <memory>
#include <array>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <functional>

#include <SDL2/SDL_log.h>

#include "shared/handle.hpp"
#include "shared/utilities.hpp"

#include "assets/file_watcher.hpp"

namespace assets
{

//template <typename container_type = std::vector<std::pair<std::uint32_t, std::unique_ptr<asset>>> >
class asset_store
{
public:
    typedef std::vector<std::pair<std::uint32_t, std::unique_ptr<asset>>> container_type;

    asset_store();
    virtual ~asset_store();

    template <class T>
    const T& operator()(const handle<T>& handle);

    template <class T, typename... Args>
    handle<T> create(const std::string& identifier, Args... args);

    void initialize();
    void report_contents() const;
private:
    file_watcher asset_file_watcher;

    container_type contents;
};

template<typename T>
inline const T& assets::asset_store::operator()(const handle<T>& handle)
{
    return std::cref(contents[handle.index].second);
}

template<class T, typename... Args>
inline handle<T> assets::asset_store::create(const std::string& identifier, Args... args)
{
    std::uint32_t next = contents.size();

    auto asset = std::make_unique<T>(identifier);

    if (!asset->loadFromDisk(std::forward<Args>(args)...))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "asset could not be loaded: \"%s\"", identifier.c_str());
    } else
    {
        contents.emplace_back(0, std::move(asset));
    }

    return handle<T>(next, 0);
}

}



#endif /* UTILITIES_DATA_STORE_HPP */
