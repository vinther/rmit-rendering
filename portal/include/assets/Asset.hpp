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

class Asset
{
public:
    enum Type
    {
        TYPE_NONE = 0,
        TYPE_MODEL,
        TYPE_SHADER,
        TYPE_SOUND,

        TYPE_CUSTOM=255,
    };

    Asset(const std::string& name, Type type);
    virtual ~Asset();

    virtual void reload();

    std::string name;
    size_t hash;
    Type type;
};


#endif /* ASSET_HPP_ */
