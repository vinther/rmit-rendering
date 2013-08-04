/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/GLBufferedMaterial.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/GLResourceManager.hpp"
#include "renderer/GLBufferedTexture.hpp"

#include "assets/Texture.hpp"
#include "assets/Shader.hpp"

#include "Utilities.hpp"

class GLResourceManager;

GLBufferedMaterial::GLBufferedMaterial()
    : texAmbient(0)
    , texDiffuse(0)
    , texSpecular(0)
    , texNormal(0)
    , texBump(0)
    , shaderHash(0)
{

}

GLBufferedMaterial::~GLBufferedMaterial()
{
    // TODO Auto-generated destructor stub
}

void GLBufferedMaterial::activate(GLResourceManager& resourceManager) const
{
    if (texDiffuse) {
        const auto& tex = resourceManager.getByHash<GLBufferedTexture>(texDiffuse);

        if (tex.tex)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex.tex);
        }
    }
}

void GLBufferedMaterial::loadFromAsset(std::shared_ptr<const asset_type>& asset, GLResourceManager& resourceManager)
{
    const auto& material = *(asset);

    if (material.texAmbient)
    {
        resourceManager.getByAsset<GLBufferedTexture>(material.texAmbient);
        texAmbient = material.texAmbient->hash;
    }

    if (material.texDiffuse)
    {
        resourceManager.getByAsset<GLBufferedTexture>(material.texDiffuse);
        texDiffuse = material.texDiffuse->hash;
    }


    if (material.texSpecular)
    {
        resourceManager.getByAsset<GLBufferedTexture>(material.texSpecular);
        texSpecular = material.texSpecular->hash;
    }


    if (material.texBump)
    {
        resourceManager.getByAsset<GLBufferedTexture>(material.texBump);
        texBump = material.texBump->hash;
    }

    materialInfo.diffuse = material.materialInfo.diffuse;
}
