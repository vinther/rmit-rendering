/*
 * Intersection.hpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#ifndef INTERSECTIONS_HPP_
#define INTERSECTIONS_HPP_

#include <glm/glm.hpp>

#include "physics/Types.hpp"

#include "Utilities.hpp"

namespace physics
{

extern "C"
{
    int triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3]);
}

inline bool rayAABBIntersection(
        const glm::vec3& origin,
        const glm::vec3& direction,
        const glm::vec3& aabbMin,
        const glm::vec3& aabbMax,
        float& distance)
{
    using glm::max;
    using glm::min;

    // r.dir is unit direction vector of ray
    glm::vec3 dirfrac = 1.0f / direction;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (aabbMin.x - origin.x) * dirfrac.x;
    float t2 = (aabbMax.x - origin.x) * dirfrac.x;
    float t3 = (aabbMin.y - origin.y) * dirfrac.y;
    float t4 = (aabbMax.y - origin.y) * dirfrac.y;
    float t5 = (aabbMin.z - origin.z) * dirfrac.z;
    float t6 = (aabbMax.z - origin.z) * dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
    if (tmax < 0)
    {
        distance = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        distance = tmax;
        return false;
    }

    distance = tmin;
    return true;
}

const float inf = std::numeric_limits<float>::infinity();

#define storess(ss,mem)     _mm_store_ss((float * const)(mem),(ss)
#define rotatelps(ps)       _mm_shuffle_ps((ps),(ps), 0x39) // a,b,c,d -> b,c,d,a
#define muxhps(low,high)    _mm_movehl_ps((low),(high))

inline bool rayAABBIntersectionOpt(
        const glm::simdVec4& origin,
        const glm::simdVec4& directionReciproc,
        const glm::simdVec4& aabbMin,
        const glm::simdVec4& aabbMax
)
{
    using glm::max;
    using glm::min;

    const glm::simdVec4 plusInf = glm::simdVec4(inf, inf, inf, inf);
    const glm::simdVec4 minInf = glm::simdVec4(-inf, -inf, -inf, -inf);

    const glm::simdVec4 l1 = (aabbMin - origin) * directionReciproc;
    const glm::simdVec4 l2 = (aabbMax - origin) * directionReciproc;

    const glm::simdVec4 fl1a = l1 - plusInf;
    const glm::simdVec4 fl2a = l2 - plusInf;

    const glm::simdVec4 fl1b = l1 + minInf;
    const glm::simdVec4 fl2b = l2 + minInf;

    glm::simdVec4 lmax = glm::max(fl1a, fl2a);
    glm::simdVec4 lmin = glm::min(fl1b, fl2b);

    const glm::simdVec4 lmax0(rotatelps(lmax.Data));
    const glm::simdVec4 lmin0(rotatelps(lmin.Data));
    lmax = _mm_min_ss(lmax.Data, lmax0.Data);
    lmin = _mm_max_ss(lmin.Data, lmin0.Data);

    const glm::simdVec4 lmax1(muxhps(lmax.Data,lmax.Data));
    const glm::simdVec4 lmin1(muxhps(lmin.Data,lmin.Data));
    lmax = _mm_min_ss(lmax.Data, lmax1.Data);
    lmin = _mm_max_ss(lmin.Data, lmin1.Data);

    const bool ret = _mm_comige_ss(lmax.Data, _mm_setzero_ps()) & _mm_comige_ss(lmax.Data,lmin.Data);

    return ret;
}

inline bool pointAABBIntersection(const glm::vec3& point, const AABB& aabb)
{
    return (point.x < aabb.max.x && point.y < aabb.max.y && point.z < aabb.max.z &&
            point.x > aabb.min.x && point.y > aabb.min.y && point.z > aabb.min.z);
}

inline bool triangleAABBIntersection(const Triangle& tri, const AABB& aabb)
{
    if (pointAABBIntersection(tri.vertices[0], aabb) ||
        pointAABBIntersection(tri.vertices[1], aabb) ||
        pointAABBIntersection(tri.vertices[2], aabb))
    {
        return true;
    } else
    {
        Triangle triCopy = tri;
        glm::vec3 boxhalfsize = (aabb.max - aabb.min) * 0.5f;
        glm::vec3 boxcenter = aabb.min + boxhalfsize;

        return 1 == triBoxOverlap((float*) &boxcenter, (float*) &boxhalfsize, (float (*)[3]) &triCopy);
    }
}


inline bool lineTriangleIntersection
(
    glm::simdVec4 const & orig, glm::simdVec4 const & dir,
    glm::simdVec4 const & vert0, glm::simdVec4 const & vert1, glm::simdVec4 const & vert2,
    glm::simdVec4 & position
)
{
    const float Epsilon = std::numeric_limits<float>::epsilon();

    glm::simdVec4 edge1 = vert1 - vert0;
    glm::simdVec4 edge2 = vert2 - vert0;

    glm::simdVec4 pvec = glm::cross(dir, edge2);

    float det = glm::dot(edge1, pvec);

    if (det > -Epsilon && det < Epsilon)
        return false;

    float inv_det = 1.0f / det;

    glm::simdVec4 tvec = orig - vert0;
    glm::simdVec4 qvec = glm::cross(tvec, edge1);

    position = glm::simdVec4(glm::dot(edge2, qvec) * inv_det, glm::dot(tvec, pvec) * inv_det, glm::dot(dir, qvec) * inv_det, 0.0f);

    if (position.y < 0.0f || position.y > 0.0f)
        return false;

    if (position.z < 0.0f || position.y + position.z > 1.0f)
        return false;

    return true;
}

}

#endif /* INTERSECTIONS_HPP_ */
