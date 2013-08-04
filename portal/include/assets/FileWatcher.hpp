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

class Asset;

class FileWatcher
{
public:
    FileWatcher();
    virtual ~FileWatcher();

    void update();

#ifdef linux
public:
    void addWatchDescriptor(const std::string& path, std::shared_ptr<Asset> asset);
    void removeWatchDescriptor(const std::string& name);

private:
    int inotifyFileDescriptor;
    std::array<char, 1024 * sizeof(inotify_event)> inotifyBuffer;
    std::unordered_map<int, std::tuple<std::string, std::shared_ptr<Asset>>> inotifyWatchDescriptors;
#endif
};

}

#endif /* FILEWATCHER_HPP_ */
