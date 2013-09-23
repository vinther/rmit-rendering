#ifndef DISK_FILE_HPP_
#define DISK_FILE_HPP_

#include "assets/asset.hpp"

#include <string>

namespace assets
{

class DiskFile: public asset
{
public:
    DiskFile(const std::string& name);
    virtual ~DiskFile();

    bool loadFromDisk(const std::string& path);

    size_t reportSize() const;
    void reload();

    std::string contents;
    std::string path;
};

}

#endif /* DISK_FILE_HPP_ */
