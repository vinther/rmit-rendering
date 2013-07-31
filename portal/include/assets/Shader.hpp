/*
 * Shader.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "assets/Asset.hpp"

#include <string>

typedef unsigned int GLuint;
typedef int GLint;

class AssetManager;

class Shader: public Asset
{
public:
    Shader(const std::string& name);
    virtual ~Shader();

    bool loadFromDisk(const std::string& path, const std::string& vertPath, const std::string& fragPath);
    size_t reportSize() const;

    void reload();

    struct RenderInfo
    {
        GLint model, view, projection;
    } renderInfo;

    GLuint program;
private:
    std::string getStringFromFile(const std::string& path);

    std::string vertPath, fragPath;
};

#endif /* SHADER_HPP_ */
