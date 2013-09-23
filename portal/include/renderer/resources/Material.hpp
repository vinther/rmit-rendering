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
#include <array>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "assets/Material.hpp"
#include "renderer/resources/AssetResource.hpp"

typedef unsigned int GLuint;
typedef int GLint;

namespace renderer
{

namespace resources
{

class Material: public AssetResource<assets::material>
{
public:
    Material(std::shared_ptr<const asset_type> asset);
    virtual ~Material();

    struct MaterialInfo
    {
        MaterialInfo();

        enum TextureBuffers
        {
            TEXTURE_EMISSIVE = 0,
            TEXTURE_AMBIENT,
            TEXTURE_DIFFUSE,
            TEXTURE_SPECULAR,
            TEXTURE_BUMP,

            TEXTURE_LAST_INDEX,
        };

        std::array<GLuint, TextureBuffers::TEXTURE_LAST_INDEX> buffers;

        glm::vec4 emission;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    } materialInfo;

    bool translucent;

    void activate() const;
    void loadFromAsset(storage_backend& resourceManager);
};

}

}

#endif /* GLBUFFEREDMATERIAL_HPP_ */
