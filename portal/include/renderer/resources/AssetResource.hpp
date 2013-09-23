/*
 * GLBufferObject.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef RENDERER_RESOURCES_ASSETRESOURCE_HPP_
#define RENDERER_RESOURCES_ASSETRESOURCE_HPP_

#include <memory>

#include <SDL2/SDL.h>

namespace renderer
{

class storage_backend;

namespace resources
{

template <class T>
class AssetResource
{
public:
	typedef T asset_type;

	enum State
    {
        PRISTINE    = 0x01,
        DIRTY       = 0x02,
        READY       = 0x04
    };

	AssetResource(std::shared_ptr<const asset_type> asset)
        : asset(asset)
		, currentVersion(0)
		, state(State::PRISTINE)
    {
	    if (asset)
	        currentVersion = asset->version;
    }

    virtual ~AssetResource() {};

    virtual void loadFromAsset(storage_backend& resourceManager) = 0;

    std::shared_ptr<const T> asset;
    size_t currentVersion;

    Uint16 state;

    bool checkAssetVersion() const
    {
        return (asset) ? asset->version <= currentVersion : false;
    }
};

}

}

#endif /* RENDERER_RESOURCES_ASSETRESOURCE_HPP_ */
