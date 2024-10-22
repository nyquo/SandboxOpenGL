#include "BasicRenderer.hpp"

#include <Logger.hpp>

namespace core {

BasicRenderer::BasicRenderer()
{
    m_modelShader = std::make_unique<core::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                                   std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag");
}

void BasicRenderer::beginFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void BasicRenderer::renderScene(const Scene& scene, std::shared_ptr<Camera> camera) const
{
    for(auto it = scene.cbegin(); it != scene.cend(); ++it)
    {
        m_modelShader->bind();
        m_modelShader->setMat4("view", camera->getView());
        m_modelShader->setMat4("projection", camera->getProjection());
        m_modelShader->setMat4("model", glm::mat4(1.0f));
        (*it)->draw(*m_modelShader);
    }
}

void BasicRenderer::endFrame() {}

void BasicRenderer::setViewport(int width, int height, int x, int y) {}

}
