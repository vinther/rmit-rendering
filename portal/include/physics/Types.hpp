/*
 * Types.hpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#ifndef PHYSICS_TYPES_HPP_
#define PHYSICS_TYPES_HPP_

#include <array>

#include <glm/glm.hpp>

namespace physics
{

struct Triangle
{
    std::array<glm::vec3, 3> vertices;
};

struct Ray
{
    glm::vec3 position;
    glm::vec3 direction;
};

struct AABB
{
    glm::vec3 min = glm::vec3(0.0f), max = glm::vec3(0.0f);
};

}

#endif /* PHYSICS_TYPES_HPP_ */
