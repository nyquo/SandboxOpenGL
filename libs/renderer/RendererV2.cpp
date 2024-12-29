#include "RendererV2.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

RendererV2::RendererV2()
{
    m_simulatedEntityShader =
      std::make_unique<Shader>(fs::path(RESSOURCES_FOLDER) / "shaders" / "SimulatedEntity.vert",
                               fs::path(RESSOURCES_FOLDER) / "shaders" / "SimulatedEntity.frag");
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

RendererV2::~RendererV2() {}

void RendererV2::beginFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    glScissor(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST); // move to endFrame()?
}

void RendererV2::renderScene(SceneV2& scene)
{
    m_simulatedEntityShader->bind();
    m_simulatedEntityShader->setMat4("view", scene.getActiveCamera()->getView());
    m_simulatedEntityShader->setMat4("projection", scene.getActiveCamera()->getProjection());
    m_square.getVertexArray().bind();
    for(auto& entity : scene.getEntites())
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0F), entity.m_position);
        m_simulatedEntityShader->setMat4("model", model);
        m_simulatedEntityShader->setVec3("squareColor", entity.m_color);
        glDrawElements(GL_TRIANGLES, m_square.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void RendererV2::endFrame() {}

void RendererV2::setViewport(int x, int y, int width, int height)
{
    m_viewportData.x = x;
    m_viewportData.y = y;
    m_viewportData.width = width;
    m_viewportData.height = height;
}

}
