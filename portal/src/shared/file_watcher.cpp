/*
 * FileWatcher.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/file_watcher.hpp"

#include <unordered_set>

#include <SDL2/SDL_log.h>

#include "assets/asset.hpp"

assets::file_watcher::file_watcher()
{
#ifdef linux
    inotifyFileDescriptor = inotify_init1(IN_NONBLOCK);

    if (0 > inotifyFileDescriptor)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize inotify");
#endif
}

assets::file_watcher::~file_watcher()
{
}

void assets::file_watcher::update()
{
#ifdef linux
    ssize_t length = read(inotifyFileDescriptor, inotifyBuffer.data(), inotifyBuffer.size());

    if (0 > length && 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Error while reading from inotify (length < 0)");
        return;
    }

    std::unordered_set<int> reloadedWatchDescriptors;
    ssize_t i = 0;
    while (i < length)
    {
        inotify_event *event = (inotify_event*) (inotifyBuffer.data() + i);

        if ((event->mask & IN_MODIFY) &&
                (reloadedWatchDescriptors.find(event->wd) == reloadedWatchDescriptors.cend()))
        {
            std::get<1>(inotifyWatchDescriptors[event->wd])->reload();

            reloadedWatchDescriptors.insert(event->wd);
        }


        i += sizeof(inotify_event) + event->len;
    }
#endif
}

#ifdef linux
void assets::file_watcher::addWatchDescriptor(const std::string& path, std::shared_ptr<asset> asset)
{
    int watchDescriptor = inotify_add_watch(inotifyFileDescriptor, ("assets/" + path).c_str(), IN_MODIFY);

    if (0 > watchDescriptor)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to add inotify watch for asset \"%s\"", path.c_str());
        return;
    }

    inotifyWatchDescriptors[watchDescriptor] = std::make_tuple(path, asset);
}
#endif
