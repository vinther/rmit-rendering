/*
 * DebugRenderer.cpp
 *
 *  Created on: 08/08/2013
 *      Author: svp
 */

#include "renderer/DebugRenderer.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "physics/Octree.hpp"

#include "Utilities.hpp"

namespace renderer
{

renderer::DebugRenderer::DebugRenderer()
{
    // TODO Auto-generated constructor stub

}

renderer::DebugRenderer::~DebugRenderer()
{
    // TODO Auto-generated destructor stub
}

void renderIntersectionTree(
        const physics::Octree& tree,
        unsigned int depth,
        const renderer::DebugRenderer::Settings& settings)
{
    UNUSED(depth);
    UNUSED(settings);

    for (const auto& aabbs: tree.data.aabbs)
    {
        for (const auto& aabb: aabbs)
        {
            glBegin(GL_LINE_STRIP);
            glVertex3f(aabb.min.x, aabb.min.y, aabb.min.z); // v0
            glVertex3f(aabb.max.x, aabb.min.y, aabb.min.z); // v1

            glVertex3f(aabb.max.x, aabb.max.y, aabb.min.z); // v2
            glVertex3f(aabb.min.x, aabb.max.y, aabb.min.z); // v3

            glVertex3f(aabb.min.x, aabb.min.y, aabb.min.z); // v0
            glEnd();

            glBegin(GL_LINE_STRIP);
            glVertex3f(aabb.min.x, aabb.min.y, aabb.max.z); // v0
            glVertex3f(aabb.max.x, aabb.min.y, aabb.max.z); // v1

            glVertex3f(aabb.max.x, aabb.max.y, aabb.max.z); // v2
            glVertex3f(aabb.min.x, aabb.max.y, aabb.max.z); // v3

            glVertex3f(aabb.min.x, aabb.min.y, aabb.max.z); // v0
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(aabb.min.x, aabb.min.y, aabb.min.z); // v0
            glVertex3f(aabb.min.x, aabb.min.y, aabb.max.z); // v0
            glVertex3f(aabb.max.x, aabb.min.y, aabb.min.z); // v1
            glVertex3f(aabb.max.x, aabb.min.y, aabb.max.z); // v1

            glVertex3f(aabb.max.x, aabb.max.y, aabb.min.z); // v2
            glVertex3f(aabb.max.x, aabb.max.y, aabb.max.z); // v2
            glVertex3f(aabb.min.x, aabb.max.y, aabb.min.z); // v3
            glVertex3f(aabb.min.x, aabb.max.y, aabb.max.z); // v3

            glEnd();
        }
    }
}

void DebugRenderer::initialize()
{
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
}

void DebugRenderer::render(const scene::Scene& scene)
{
    if (!settings.enabled)
        return;

    const auto& camera = *(scene.camera);
    const glm::mat4 viewProjectionMatrix = camera.getViewProjectionMatrix();

    glColor3f(1.0f, 0.0f, 1.0f);

    glLineWidth(10.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(viewProjectionMatrix));


//    const auto pos = camera.position + camera.forward() * 100.0f;

//    glColor3f(0.0f, 1.0f, 0.0f);
//    glBegin(GL_LINES);
//        glColor3f(1,0,0);
//        glVertex3f(0.0f, 200.0f, 0.0f);
//
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(pos.x, pos.y, pos.z);
//    glEnd();

    glLineWidth(1.0f);

    glColor3f(1.0f, 0.0f, 1.0f);
    //renderIntersectionTree(*(scene.intersectionTree), 0, settings);

    glLineWidth(10.0f);
    glPointSize(10.0f);
    glBegin(GL_LINES);
    for (const auto& point: points)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 200.0f, 0.0f);

        glColor3f(1,0,0);
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();


    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

} /* namespace threading */
