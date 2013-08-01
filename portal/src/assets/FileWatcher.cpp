/*
 * FileWatcher.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/FileWatcher.hpp"

#include <unordered_set>

#include <SDL2/SDL.h>

#include "assets/Asset.hpp"

FileWatcher::FileWatcher()
{
#ifdef linux
    inotifyFileDescriptor = inotify_init1(IN_NONBLOCK);

    if (0 > inotifyFileDescriptor)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize inotify");
#endif
}

FileWatcher::~FileWatcher()
{
}

void FileWatcher::update()
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

        if (event->mask & IN_MODIFY && (reloadedWatchDescriptors.find(event->wd) == reloadedWatchDescriptors.cend()))
        {
            std::get<1>(inotifyWatchDescriptors[event->wd])->reload();

            reloadedWatchDescriptors.insert(event->wd);
        }


        i += sizeof(inotify_event) + event->len;
    }
#endif
}

#ifdef linux
void FileWatcher::addWatchDescriptor(const std::string& path, std::shared_ptr<Asset> asset)
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
