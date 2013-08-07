/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/BufferedMaterial.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/ResourceManager.hpp"
#include "renderer/BufferedTexture.hpp"

#include "assets/Texture.hpp"
#include "assets/Shader.hpp"

#include "Utilities.hpp"

renderer::BufferedMaterial::BufferedMaterial()
    : texAmbient(0)
    , texDiffuse(0)
    , texSpecular(0)
    , texNormal(0)
    , texBump(0)
    , shaderHash(0)
    , translucent(false)
{

}

renderer::BufferedMaterial::~BufferedMaterial()
{
    // TODO Auto-generated destructor stub
}

void renderer::BufferedMaterial::activate(ResourceManager& resourceManager) const
{
    if (texAmbient) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, resourceManager.getByHash<BufferedTexture>(texAmbient).tex);
    }

    if (texDiffuse) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, resourceManager.getByHash<BufferedTexture>(texDiffuse).tex);
    }

    if (texSpecular) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, resourceManager.getByHash<BufferedTexture>(texSpecular).tex);
    }

    if (texNormal) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, resourceManager.getByHash<BufferedTexture>(texNormal).tex);
    }

    if (texBump) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, resourceManager.getByHash<BufferedTexture>(texBump).tex);
    }
}

void renderer::BufferedMaterial::loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager)
{
    const auto& material = *(asset);

    if (material.texAmbient)
    {
        resourceManager.getByAsset<BufferedTexture>(material.texAmbient);
        texAmbient = material.texAmbient->hash;
    }

    if (material.texDiffuse)
    {
        resourceManager.getByAsset<BufferedTexture>(material.texDiffuse);
        texDiffuse = material.texDiffuse->hash;
    }


    if (material.texSpecular)
    {
        resourceManager.getByAsset<BufferedTexture>(material.texSpecular);
        texSpecular = material.texSpecular->hash;
    }

    if (material.texNormal)
    {
        resourceManager.getByAsset<BufferedTexture>(material.texNormal);
        texSpecular = material.texNormal->hash;
    }

    if (material.texBump)
    {
        resourceManager.getByAsset<BufferedTexture>(material.texBump);
        texBump = material.texBump->hash;
    }

    materialInfo.emission = material.materialInfo.emission;
    materialInfo.ambient = material.materialInfo.ambient;
    materialInfo.diffuse = material.materialInfo.diffuse;
    materialInfo.specular = material.materialInfo.specular;
    materialInfo.shininess = material.materialInfo.shininess;

    translucent = material.translucent;
}
