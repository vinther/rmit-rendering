#ifndef SHARED_UTILITIES_HPP_
#define SHARED_UTILITIES_HPP_

#include <memory>

#include <SDL2/SDL.h>

namespace std
{

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#define UNUSED(x) (void)(x)

namespace client
{

enum LogCategories: int
{
    PORTAL_LOG_CATEGORY_ASSETS = SDL_LOG_CATEGORY_CUSTOM,
    PORTAL_LOG_CATEGORY_CLIENT,
    PORTAL_LOG_CATEGORY_INPUT,
    PORTAL_LOG_CATEGORY_PHYSICS,
    PORTAL_LOG_CATEGORY_RENDERER,
    PORTAL_LOG_CATEGORY_SCENE,
    PORTAL_LOG_CATEGORY_THREADING,
};

}


#endif /* SHARED_UTILITIES_HPP_ */
