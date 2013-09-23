#ifndef DISK_FILE_HPP_
#define DISK_FILE_HPP_

#include "assets/asset.hpp"

#include <string>

namespace assets
{

class file: public asset
{
public:
    file(const std::string& name);
    virtual ~file();

    bool loadFromDisk(const std::string& path);

    size_t reportSize() const;
    void reload();

    std::string contents;
    std::string path;
};

}

#endif /* DISK_FILE_HPP_ */
