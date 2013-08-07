/*
 * Material.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef GLBUFFEREDMATERIAL_HPP_
#define GLBUFFEREDMATERIAL_HPP_

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "renderer/BufferObject.hpp"
#include "assets/Material.hpp"

class aiMaterial;

typedef unsigned int GLuint;

namespace renderer
{

class BufferedMaterial: public BufferObject<assets::Material>
{
public:
    BufferedMaterial();
    virtual ~BufferedMaterial();

    struct MaterialInfo
    {
        glm::vec4 emission;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    } materialInfo;

    size_t texAmbient;
    size_t texDiffuse;
    size_t texSpecular;
    size_t texNormal;
    size_t texBump;

    size_t shaderHash;

    bool translucent;

    void activate(ResourceManager& resourceManager) const;
    void loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager);
};

}

#endif /* GLBUFFEREDMATERIAL_HPP_ */
