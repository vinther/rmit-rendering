/*
 * Asset.hpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#ifndef ASSET_HPP_
#define ASSET_HPP_

#include <string>
#include <functional>
#include <vector>

namespace assets
{

class Asset
{
public:
    enum Type
    {
        TYPE_NONE = 0,
        TYPE_MODEL,
        TYPE_SHADER,
        TYPE_SOUND,
        TYPE_TEXTURE,
        TYPE_MATERIAL,

        TYPE_CUSTOM=255,
    };

    virtual ~Asset();

    virtual void reload() = 0;
    virtual size_t reportSize() const = 0;

    std::string name;
    size_t hash;
    Type type;

    /*
     * Relative paths to every file that makes up this asset.
     *
     * If assets fill this vector with paths it will be picked up by the file watcher and get reloaded upon changes
     */
    std::vector<std::string> files;
    size_t version;
protected:
    Asset(const std::string& name, Type type);

    std::string getStringFromFile(const std::string& path) const;
};

}

#endif /* ASSET_HPP_ */
