/*
 * Asset.hpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#ifndef ASSET_HPP_
#define ASSET_HPP_

#include <assimp/scene.h>

class Asset
{
public:
    Asset(const aiScene* scene);


    const aiScene* scene;
};


#endif /* ASSET_HPP_ */
