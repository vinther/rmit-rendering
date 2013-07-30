/*
 * Material.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "assets/Asset.hpp"

class Material: public Asset
{
public:
    Material(const std::string& name);
    virtual ~Material();

    bool loadFromDisk(const std::string& name, AssetManager& assetManager);
    size_t reportSize() const;

    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
    float Tr;

    Uint8 illum;

//    std::string map_Ka;
//    std::string map_Kd;
//    std::string map_Ks;
//    std::string map_Ns;
//    std::string map_d;
//    std::string map_bump;
//
//    std::weak_ptr<Asset> shader;
};

#endif /* MATERIAL_HPP_ */
