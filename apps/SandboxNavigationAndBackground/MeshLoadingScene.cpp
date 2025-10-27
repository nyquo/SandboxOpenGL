#include "MeshLoadingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/Model.hpp>

MeshLoadingScene::MeshLoadingScene()
{
    m_basicModelShader =
      std::make_shared<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BasicModelShader.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/BasicModelShader.frag");
    m_camera = std::make_shared<renderer::PerspectiveCamera>(m_width, m_height, glm::vec3(0.0F, 0.0F, 10.0F));
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

MeshLoadingScene::~MeshLoadingScene() {}

void MeshLoadingScene::renderScene()
{
    beginFrame();

    if(m_model)
    {
        m_basicModelShader->bind();

        m_basicModelShader->setMat4("view", m_camera->getView());
        m_basicModelShader->setMat4("projection", m_camera->getProjection());
        m_basicModelShader->setVec3("viewPos", m_camera->getPosition());
        m_basicModelShader->setMat4("model", glm::mat4(1.0f));

        m_model->draw(*m_basicModelShader);
    }

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

void MeshLoadingScene::loadMeshFromFile(fs::path path)
{
    m_model = nullptr;
    if(path.empty())
    {
        core::Logger::logWarning("Removing previous mesh (no path provided).");
        return;
    }
    else
    {
        core::Logger::logInfo("Loading mesh from path: ", path);
        m_model = std::make_shared<renderer::Model>(path);
    }
}
