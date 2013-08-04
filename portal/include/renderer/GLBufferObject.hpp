/*
 * GLBufferObject.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef GLBUFFEROBJECT_HPP_
#define GLBUFFEROBJECT_HPP_

#include <memory>

#include <SDL2/SDL.h>


namespace assets
{
class AssetManager;
}

class GLResourceManager;

template <class T>
class GLBufferObject
{
public:
    enum State
    {
        PRISTINE    = 0x01,
        DIRTY       = 0x02,
        READY       = 0x04,
    };

    typedef T asset_type;

    GLBufferObject()
        : state(State::PRISTINE)
        , currentVersion(0)
     {}

    virtual ~GLBufferObject()
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

    virtual void loadFromAsset(std::shared_ptr<const asset_type>& asset, GLResourceManager& resourceManager) = 0;
protected:
    size_t currentVersion;
    std::weak_ptr<const T> asset;
};

#endif /* GLBUFFEROBJECT_HPP_ */
