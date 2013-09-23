#ifndef UTILITIES_DATA_STORE_HPP
#define UTILITIES_DATA_STORE_HPP

#include <memory>
#include <array>
#include <string>
#include <unordered_map>
#include <type_traits>

#include <SDL2/SDL_log.h>

#include "shared/handle.hpp"
#include "shared/utilities.hpp"

namespace utilities
{

class file_watcher;

template <typename T, typename container_type = std::vector<T> >
class data_store
{
public:
    data_store();
    virtual ~data_store();

    const T& operator()(const Handle<T>& handle);

    template <class T, typename... Args>
    handle<T> create(Args... args);

    void initialize();
    void report_contents() const;
private:
    handle<file_watcher> file_watcher;

    container_type contents;
};

template<class T>
inline const T& utilities::data_store::operator()(const utilities::handle<T>& handle)
{
    return std::ref(assets[handle.index]);
}

template<class T, typename... Args>
inline Handle<T> assets::data_store::create(const std::string& name, Args... args)
{
    std::uint32_t next = assets.size();
    auto asset = std::make_unique<T>(name);

    if (!asset->loadFromDisk(std::forward<Args>(args)...))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "asset could not be loaded: \"%s\"", name.c_str());
    } else
    {
        assets.emplace_back(0, std::move(asset));
    }

    return Handle<T>(next, 0);
}

}

#endif /* UTILITIES_DATA_STORE_HPP */
