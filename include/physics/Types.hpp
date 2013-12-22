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

#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>

namespace physics
{

template <typename T>
struct TriangleT
{
    const T& operator[](int idx) const
    { return vertices[idx]; }

    T& operator[](int idx)
    { return vertices[idx]; }

    std::array<T, 3> vertices;
};

typedef TriangleT<glm::vec3> Triangle;

template <typename T>
struct RayT
{
    T origin;
    T direction;
};

typedef RayT<glm::vec3> Ray;

template <typename T>
struct LineT
{
    T origin;
    T end;
};

typedef LineT<glm::vec3> Line;

template <typename T>
struct AABBT
{
    T min;
    T max;
};

typedef AABBT<glm::vec3> AABB;

struct IntersectionPoint
{
    glm::vec3 normal;
    glm::vec3 position;
};

}

#endif /* PHYSICS_TYPES_HPP_ */