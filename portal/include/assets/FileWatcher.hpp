/*
 * FileWatcher.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef FILEWATCHER_HPP_
#define FILEWATCHER_HPP_

#include <memory>
#include <array>
#include <tuple>
#include <string>
#include <unordered_map>

#ifdef linux
#include <unistd.h>
#include <sys/inotify.h>
#endif

namespace assets
{

class asset;

class file_watcher
{
public:
    file_watcher();
    virtual ~file_watcher();

    void update();

    void addWatchDescriptor(const std::string& path, std::shared_ptr<asset> asset);
    void removeWatchDescriptor(const std::string& name);

#ifdef linux
private:
    int inotifyFileDescriptor;
    std::array<char, 1024 * sizeof(inotify_event)> inotifyBuffer;
    std::unordered_map<int, std::tuple<std::string, std::shared_ptr<asset>>> inotifyWatchDescriptors;
#endif
};

}

#endif /* FILEWATCHER_HPP_ */
