/*
 * Material.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "assets/Asset.hpp"

class Shader;
class Texture;

class Material
{
public:
    Material(const std::string& name);
    virtual ~Material();

    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
    float Tr;

    std::weak_ptr<Texture> texAmbient;
    std::weak_ptr<Texture> texDiffuse;
    std::weak_ptr<Texture> texSpecular;
    std::weak_ptr<Texture> texNormal;
    std::weak_ptr<Texture> texBump;

    std::weak_ptr<Shader> shader;
};

#endif /* MATERIAL_HPP_ */
