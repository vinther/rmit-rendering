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

#include <assimp/scene.h>

class AssetManager;

class Asset
{
public:
    enum Type
    {
        TYPE_NONE = 0,
        TYPE_MODEL,
        TYPE_SHADER,
        TYPE_SOUND,
        TYPE_MATERIAL,

        TYPE_CUSTOM=255,
    };


    virtual ~Asset();

    virtual bool loadFromDisk(const std::string& name, AssetManager& assetManager) = 0;
    virtual size_t reportSize() const = 0;

    std::string name;
    size_t hash;
    Type type;

protected:
    Asset(const std::string& name, Type type);
};


#endif /* ASSET_HPP_ */
