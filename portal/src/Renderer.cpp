/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "Renderer.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/Scene.hpp"
#include "scene/Camera.hpp"
#include "assets/AssetManager.hpp"

Renderer::Renderer()
{
    // TODO Auto-generated constructor stub

}

Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

void Renderer::render(const Scene& scene, RenderResults& results)
{
    results.settings = settings;

    const auto& camera = *(scene.camera);
    const auto sceneObject = scene.assetManager->get<Asset>("models/shuttle.obj");

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glm::mat4 MVM = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);

    glLoadMatrixf(glm::value_ptr(MVM));



    if (!sceneObject.expired() && indices.empty())
    {
        auto d = sceneObject.lock();

        indices.resize(d->scene->mNumMeshes);
        for (unsigned int i = 0; i < d->scene->mNumMeshes; ++i)
        {
            for (unsigned int j = 0; j < d->scene->mMeshes[i]->mNumFaces; ++j)
            {
                std::copy(d->scene->mMeshes[i]->mFaces[j].mIndices, d->scene->mMeshes[i]->mFaces[j].mIndices + 3, std::back_inserter(indices[i]));
            }
        }

    }

    static glm::vec3 cols[10] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 0.5f, 1.0f),
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.5f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 0.0f, 0.5f),
    };

    if (!sceneObject.expired())
    {
        auto d = sceneObject.lock();

        glEnableClientState(GL_VERTEX_ARRAY);

        for (unsigned int i = 0; i < d->scene->mNumMeshes; ++i)
        {
            glColor3fv(glm::value_ptr(cols[i]));
            glVertexPointer(3, GL_FLOAT, 0, d->scene->mMeshes[i]->mVertices);
            glDrawElements(GL_TRIANGLES, indices[i].size(), GL_UNSIGNED_INT, indices[i].data());
        }

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glPopMatrix();
}

void Renderer::initialize()
{
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    glViewport(0, 0, (GLsizei) settings.width, (GLsizei) settings.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(settings.fov, (GLfloat) settings.width / (GLfloat) settings.height, 1.0, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}
