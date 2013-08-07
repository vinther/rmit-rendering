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
        const physics::Octree<256>::Node& node,
        unsigned int depth,
        const renderer::DebugRenderer::Settings& settings)
{
    if (depth > settings.maxTreeDepth)
        return;

    glBegin(GL_LINE_STRIP);
    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.min.z); // v0
    glVertex3f(node.aabb.max.x, node.aabb.min.y, node.aabb.min.z); // v1

    glVertex3f(node.aabb.max.x, node.aabb.max.y, node.aabb.min.z); // v2
    glVertex3f(node.aabb.min.x, node.aabb.max.y, node.aabb.min.z); // v3

    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.min.z); // v0
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.max.z); // v0
    glVertex3f(node.aabb.max.x, node.aabb.min.y, node.aabb.max.z); // v1

    glVertex3f(node.aabb.max.x, node.aabb.max.y, node.aabb.max.z); // v2
    glVertex3f(node.aabb.min.x, node.aabb.max.y, node.aabb.max.z); // v3

    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.max.z); // v0
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.min.z); // v0
    glVertex3f(node.aabb.min.x, node.aabb.min.y, node.aabb.max.z); // v0
    glVertex3f(node.aabb.max.x, node.aabb.min.y, node.aabb.min.z); // v1
    glVertex3f(node.aabb.max.x, node.aabb.min.y, node.aabb.max.z); // v1

    glVertex3f(node.aabb.max.x, node.aabb.max.y, node.aabb.min.z); // v2
    glVertex3f(node.aabb.max.x, node.aabb.max.y, node.aabb.max.z); // v2
    glVertex3f(node.aabb.min.x, node.aabb.max.y, node.aabb.min.z); // v3
    glVertex3f(node.aabb.min.x, node.aabb.max.y, node.aabb.max.z); // v3

    glEnd();

    if (!node.isLeaf)
    {
        for (const auto& child: node.children)
        {
            renderIntersectionTree(*(child), depth + 1, settings);
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
    const auto viewMatrix = glm::mat4_cast(-camera.state.rotation) * glm::translate(glm::mat4(1.0f), -camera.state.position);
    const auto projMatrix = glm::perspective(settings.fov, settings.width / (float) settings.height, settings.nearClip, settings.farClip);

    glColor3f(1.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(projMatrix));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(viewMatrix));

    renderIntersectionTree(*(scene.intersectionTree->root), 0, settings);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

} /* namespace threading */
