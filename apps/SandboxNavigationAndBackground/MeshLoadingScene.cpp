#include "MeshLoadingScene.hpp"

#include <core/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshLoadingScene::~MeshLoadingScene()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

MeshLoadingScene::MeshLoadingScene() {}

void MeshLoadingScene::renderScene()
{
    beginFrame();

    // Rendering code would go here

    endFrame();
}

void MeshLoadingScene::beginFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_x, m_y, m_width, m_height);
    glScissor(m_x, m_y, m_width, m_height);
    const auto& color = m_options.backgroundColor;
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MeshLoadingScene::endFrame() { glDisable(GL_SCISSOR_TEST); }

void MeshLoadingScene::setViewport(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}
