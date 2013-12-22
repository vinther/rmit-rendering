/*
 * log.hpp
 *
 *  Created on: 05/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_LOG_LOG_HPP_
#define CRY_TERRA_LOG_LOG_HPP_

#include <SDL2/SDL_log.h>

#include <cry_terra/utilities/ignore.hpp>

namespace cry_terra
{

namespace log
{

enum class category : int
{
    unused = SDL_LOG_CATEGORY_CUSTOM,

    file_system,
    graphics,
    sound,
    network,
    physics,

    custom,
};

template <typename... Args>
inline void log(const std::string& fmt, Args... data)
{
    SDL_Log(fmt.c_str(), data...);
}

template <typename... Args>
inline void log_error(category category, const std::string& fmt, Args... data)
{
    utilities::ignore(category);

    // SDL_LogError(static_cast<int>(category), fmt.c_str(), data...);

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, (fmt).c_str(), data...);
}

}

}

#endif /* CRY_TERRA_LOG_LOG_HPP_ */
