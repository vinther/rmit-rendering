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

    BufferObject()
        : state(State::PRISTINE)
        , currentVersion(0)
     {}

    virtual ~BufferObject()
    {}

    Uint16 state;

    bool checkAssetVersion()
    {
        if (!asset.expired())
        {
            const auto& asset = *(asset.lock());
            return asset.version <= currentVersion;
        }

        return false;
    }

    virtual void loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager) = 0;
protected:
    size_t currentVersion;
    std::weak_ptr<const T> asset;
};

}

#endif /* BUFFEROBJECT_HPP_ */
