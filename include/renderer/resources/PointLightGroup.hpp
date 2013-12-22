/*
 * PointLightGroup.hpp
 *
 *  Created on: 26/08/2013
 *      Author: svp
 */

#ifndef POINTLIGHTGROUP_HPP_
#define POINTLIGHTGROUP_HPP_

#include <memory>

#include <glm/glm.hpp>

#include "assets/Model.hpp"

#include "renderer/resources/UniformBuffer.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

namespace renderer
{

namespace resources
{

class PointLightGroup
{
public:
    struct LightData
    {
        glm::vec4 position, color, extraA;
    };

    PointLightGroup();
    PointLightGroup(const std::shared_ptr<const assets::Model> geometry, const std::initializer_list<LightData>& data);
    ~PointLightGroup();

    void create(const std::shared_ptr<const assets::Model> geometry, const std::vector<LightData>& data);

    struct MeshData
    {
        unsigned int numFaces;
        GLuint vao;
    } meshData;

    std::vector<LightData> data;

//    std::unique_ptr<UniformBuffer> buffer;
//    unsigned int count;
};


}

}



#endif /* POINTLIGHTGROUP_HPP_ */
