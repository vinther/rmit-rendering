/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/Material.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/scene.h>

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/Texture.hpp"

#include "Utilities.hpp"

Material::Material()
    : texIDTEST(0)
{

}

Material::~Material()
{
    // TODO Auto-generated destructor stub
}

void Material::activate()
{
    if (!texDiffuse.expired() && !texIDTEST)
    {
        const auto& tex = *(texDiffuse.lock());
        const auto& surface = *(tex.surface);

        glGenTextures(1, &texIDTEST);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, texIDTEST);

        if (tex.surface)
        {
            //SDL_Log("Bytes per pixel: %d, w %d, h %d", surface.format->BitsPerPixel, surface.w, surface.h);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface.w, surface.h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface.pixels);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    if (!texDiffuse.expired() && texIDTEST) {
        //const auto& tex = *(texAmbient.lock());

        glBindTexture(GL_TEXTURE_2D, texIDTEST);
        //SDL_Log("SPAM: Enabling texture with name: %s", tex.name.c_str());
    }
}

void Material::createFromAssimpMaterial(const aiMaterial& material, const Model& model, AssetManager& assetManager)
{
    aiString ambientTexture, diffuseTexture, specularTexture, normalsTexture, heightTexture;
    material.GetTexture(aiTextureType_AMBIENT, 0, &ambientTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_SPECULAR, 0, &specularTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_NORMALS, 0, &normalsTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_HEIGHT, 0, &heightTexture, nullptr, nullptr, nullptr, nullptr);

    const std::string basePath = model.basePath + "/";

    if (0 < ambientTexture.length)
        texAmbient = assetManager.getOrCreate<Texture>(std::string(ambientTexture.C_Str()), basePath + std::string(ambientTexture.C_Str()));

    if (0 < diffuseTexture.length)
        texDiffuse = assetManager.getOrCreate<Texture>(std::string(diffuseTexture.C_Str()), basePath + std::string(diffuseTexture.C_Str()));

    if (0 < specularTexture.length)
        texSpecular = assetManager.getOrCreate<Texture>(std::string(specularTexture.C_Str()), basePath + std::string(specularTexture.C_Str()));

    if (0 < normalsTexture.length)
        texNormal = assetManager.getOrCreate<Texture>(std::string(normalsTexture.C_Str()), basePath + std::string(normalsTexture.C_Str()));

    if (0 < heightTexture.length)
        texBump = assetManager.getOrCreate<Texture>(std::string(heightTexture.C_Str()), basePath + std::string(heightTexture.C_Str()));

    aiColor3D ambient, diffuse, specular;
    material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material.Get(AI_MATKEY_COLOR_SPECULAR, specular);

    materialInfo.Ka = glm::vec4(ambient.r, ambient.g, ambient.b, 0.0f);
    materialInfo.Kd = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 0.0f);
    materialInfo.Ks = glm::vec4(specular.r, specular.g, specular.b, 0.0f);

    aiString name;
    material.Get(AI_MATKEY_NAME, name);

    this->name = std::string(name.C_Str());
}
