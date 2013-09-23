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

#include "shared/utilities.hpp"

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

#ifdef __GNUC__
    #define _MM_ALIGN16 __attribute__ ((aligned (16)))
#endif

static const float _MM_ALIGN16
    ps_cst_plus_inf[4]  = {  inf,  inf,  inf,  inf },
    ps_cst_minus_inf[4] = { -inf, -inf, -inf, -inf };

#define loadps(mem)     _mm_load_ps((const float * const)(mem))
#define storess(ss,mem)     _mm_store_ss((float * const)(mem),(ss))
#define minss           _mm_min_ss
#define maxss           _mm_max_ss
#define minps           _mm_min_ps
#define maxps           _mm_max_ps
#define mulps           _mm_mul_ps
#define subps           _mm_sub_ps
#define rotatelps(ps)       _mm_shuffle_ps((ps),(ps), 0x39) // a,b,c,d -> b,c,d,a
#define muxhps(low,high)    _mm_movehl_ps((low),(high)) // low{a,b,c,d}|high{e,f,g,h

inline bool rayAABBIntersectionOpt(
        const glm::simdVec4& origin,
        const glm::simdVec4& directionReciproc,
        const glm::simdVec4& aabbMin,
        const glm::simdVec4& aabbMax
)
{
    // you may already have those values hanging around somewhere
    const __m128
        plus_inf    = loadps(ps_cst_plus_inf),
        minus_inf   = loadps(ps_cst_minus_inf);

    // use whatever's apropriate to load.
    const __m128
        box_min = loadps(&aabbMin),
        box_max = loadps(&aabbMax),
        pos = loadps(&origin),
        inv_dir = loadps(&directionReciproc);

    // use a div if inverted directions aren't available
    const __m128 l1 = mulps(subps(box_min, pos), inv_dir);
    const __m128 l2 = mulps(subps(box_max, pos), inv_dir);

    // the order we use for those min/max is vital to filter out
    // NaNs that happens when an inv_dir is +/- inf and
    // (box_min - pos) is 0. inf * 0 = NaN
    const __m128 filtered_l1a = minps(l1, plus_inf);
    const __m128 filtered_l2a = minps(l2, plus_inf);

    const __m128 filtered_l1b = maxps(l1, minus_inf);
    const __m128 filtered_l2b = maxps(l2, minus_inf);

    // now that we're back on our feet, test those slabs.
    __m128 lmax = maxps(filtered_l1a, filtered_l2a);
    __m128 lmin = minps(filtered_l1b, filtered_l2b);

    // unfold back. try to hide the latency of the shufps & co.
    const __m128 lmax0 = rotatelps(lmax);
    const __m128 lmin0 = rotatelps(lmin);
    lmax = minss(lmax, lmax0);
    lmin = maxss(lmin, lmin0);

    const __m128 lmax1 = muxhps(lmax,lmax);
    const __m128 lmin1 = muxhps(lmin,lmin);
    lmax = minss(lmax, lmax1);
    lmin = maxss(lmin, lmin1);

    const bool ret = _mm_comige_ss(lmax, _mm_setzero_ps()) & _mm_comige_ss(lmax,lmin);

    return  ret;
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
    float & t
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

    float u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f)
        return false;

    glm::simdVec4 qvec = glm::cross(tvec, edge1);

    float v = glm::dot(dir, qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = glm::dot(edge2, qvec) * inv_det;

    return true;
}

}

#endif /* INTERSECTIONS_HPP_ */
