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

void drawAABBs(
        const std::array<physics::AABB, 8>& aabbs,
        const renderer::DebugRenderer::Settings& settings)
{
    UNUSED(settings);

    for (const auto& aabb: aabbs)
    {
    	const auto min = aabb.min;
    	const auto max = aabb.max;

        glBegin(GL_LINE_STRIP);
        glVertex3f(min.x, min.y, min.z); // v0
        glVertex3f(max.x, min.y, min.z); // v1

        glVertex3f(max.x, max.y, min.z); // v2
        glVertex3f(min.x, max.y, min.z); // v3

        glVertex3f(min.x, min.y, min.z); // v0
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(min.x, min.y, max.z); // v0
        glVertex3f(max.x, min.y, max.z); // v1

        glVertex3f(max.x, max.y, max.z); // v2
        glVertex3f(min.x, max.y, max.z); // v3

        glVertex3f(min.x, min.y, max.z); // v0
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(min.x, min.y, min.z); // v0
        glVertex3f(min.x, min.y, max.z); // v0
        glVertex3f(max.x, min.y, min.z); // v1
        glVertex3f(max.x, min.y, max.z); // v1

        glVertex3f(max.x, max.y, min.z); // v2
        glVertex3f(max.x, max.y, max.z); // v2
        glVertex3f(min.x, max.y, min.z); // v3
        glVertex3f(min.x, max.y, max.z); // v3

        glEnd();
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
    const auto& camera = *(scene.camera);
    const glm::mat4 viewProjectionMatrix = camera.viewProjection();

    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(viewProjectionMatrix));

    if (settings.drawBVH)
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glLineWidth(2.0f);
        glColor3f(1.0f, 1.0f, 0.5f);

        glPushMatrix();
        glMultMatrixf(glm::value_ptr(scene.root->transformation));

        for (const auto& aabbs: scene.root->bvh->data.aabbs)
        {
            drawAABBs(aabbs, settings);
        }
        glPopMatrix();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }

    if (settings.drawRays)
    {
        glLineWidth(10.0f);

        glBegin(GL_LINES);
        for (const auto& line: lines)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(line.origin.x, line.origin.y, line.origin.z);

            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(line.end.x, line.end.y, line.end.z);
        }
        glEnd();
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if (settings.drawCrosshair)
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, 1.0f, 0.0f, 1.0f)));

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glLineWidth(4.0f);
        glColor3f(1.0f, 0.0f, 0.0f);

        const glm::vec2 size(.05f, 0.05f);
        glBegin(GL_LINES);
            glVertex2f(0.5f - size.x, 0.5f);
            glVertex2f(0.5f + size.x, 0.5f);

            glVertex2f(0.5f, 0.5f - size.y);
            glVertex2f(0.5f, 0.5f + size.y);
        glEnd();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }
}

}
