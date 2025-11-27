#include "ExploadingModelScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
ExploadingModelScene::ExploadingModelScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Exploading model", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_camera(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, 3.0F, 10.0F))
  , m_model(std::string(RESSOURCES_FOLDER) + "/Models/backpack/backpack.obj")
  , m_exploadingModelShader(std::string(RESSOURCES_FOLDER) + "/shaders/ExploadingModel.vert",
                            std::string(RESSOURCES_FOLDER) + "/shaders/ExploadingModel.frag",
                            std::string(RESSOURCES_FOLDER) + "/shaders/ExploadingModel.geom")
{
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void ExploadingModelScene::onEvent(core::Event& event) {}

void ExploadingModelScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    m_camera.setViewPortSize(getWidth(), getHeight());

    begin();
    drawScene();
    end();
}

void ExploadingModelScene::drawScene()
{
    m_exploadingModelShader.bind();
    m_exploadingModelShader.setMat4("projection", m_camera.getProjection());
    m_exploadingModelShader.setMat4("view", m_camera.getView());
    m_exploadingModelShader.setMat4("model", glm::mat4(1.0f));
    m_exploadingModelShader.setFloat("time", glfwGetTime());
    m_model.draw(m_exploadingModelShader);
}