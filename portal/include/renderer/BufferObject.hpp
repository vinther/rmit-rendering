/*
 * GLBufferObject.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef BUFFEROBJECT_HPP_
#define BUFFEROBJECT_HPP_

#include <memory>

#include <SDL2/SDL.h>

namespace assets
{
class AssetManager;
}

namespace renderer
{

class ResourceManager;

template <class T>
class BufferObject
{
public:
    enum State
    {
        PRISTINE    = 0x01,
        DIRTY       = 0x02,
        READY       = 0x04
    };

    typedef T asset_type;

    BufferObject(std::shared_ptr<const asset_type> asset)
        : asset(asset)
        , currentVersion(0)
    	, state(State::PRISTINE)
     {
    	currentVersion = asset->version;
     }

    virtual ~BufferObject()
    {}

    std::shared_ptr<const T> asset;
    size_t currentVersion;

    Uint16 state;

    bool checkAssetVersion()
    {
        return asset->version <= currentVersion;
    }

    virtual void loadFromAsset(ResourceManager& resourceManager) = 0;
};

}

#endif /* BUFFEROBJECT_HPP_ */
