/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/Renderer.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"

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

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glm::mat4 MVM = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);

    glLoadMatrixf(glm::value_ptr(MVM));

    const auto sceneObject = scene.assetManager->get<Model>("models/sibenik.obj");
    scene.assetManager->get<Model>("models/shuttle.obj");
    scene.assetManager->get<Model>("models/sibenik.obj");
    scene.assetManager->get<Model>("models/CornellBox-Sphere.obj");
    if (!sceneObject.expired())
    {
        auto& d = *(sceneObject.lock());

        bufferObject(d);

        glEnableClientState(GL_VERTEX_ARRAY);

        renderObject(d);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glPopMatrix();
}

void Renderer::initialize()
{
    glewInit();

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

void Renderer::bufferObject(Model& model)
{
    auto& renderInformation = model.renderInformation;

    if (renderInformation.state &
            (Model::RenderInformation::State::DIRTY | Model::RenderInformation::State::PRISTINE))
    {
        //SDL_Log("No rendering information saved for \"%s\", now generating buffers etc.", model.name.c_str());

        if (model.scene == nullptr)
            throw std::runtime_error("Something wrong...");

        renderInformation.vbo.resize(model.scene->mNumMeshes);
        renderInformation.ibo.resize(model.scene->mNumMeshes);
        renderInformation.numFaces.resize(model.scene->mNumMeshes);

        glGenBuffers(model.scene->mNumMeshes, renderInformation.vbo.data());
        glGenBuffers(model.scene->mNumMeshes, renderInformation.ibo.data());

        std::vector<GLuint> indices;

        for (unsigned int i = 0; i < model.scene->mNumMeshes; ++i)
        {
            const auto& mesh = *(model.scene->mMeshes[i]);

            indices.resize(mesh.mNumFaces * 3);
            for (unsigned int j = 0; j < mesh.mNumFaces; ++j)
            {
                std::copy(mesh.mFaces[j].mIndices, mesh.mFaces[j].mIndices + 3, indices.data() + j * 3);
            }

            renderInformation.numFaces[i] = mesh.mNumFaces;

            glBindBuffer(GL_ARRAY_BUFFER, renderInformation.vbo[i]);
            glBufferData(GL_ARRAY_BUFFER, mesh.mNumVertices * sizeof(aiVector3D), mesh.mVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderInformation.ibo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.mNumFaces * 3 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        }

        renderInformation.state = Model::RenderInformation::State::BUFFERED;
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

void Renderer::renderObject(const Model& model)
{
    auto& renderInformation = model.renderInformation;

    for (unsigned int i = 0; i < renderInformation.vbo.size(); ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, renderInformation.vbo[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderInformation.ibo[i]);

        glColor3fv(glm::value_ptr(cols[i % 10]));
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawElements(GL_TRIANGLES, renderInformation.numFaces[i] * 3, GL_UNSIGNED_INT, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
