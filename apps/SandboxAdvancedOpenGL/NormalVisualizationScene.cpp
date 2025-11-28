#include "NormalVisualizationScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
NormalVisualizationScene::NormalVisualizationScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Normal Visualization", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_camera(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, 0.0F, m_cameraDistance))
  , m_model(std::string(RESSOURCES_FOLDER) + "/Models/backpack/backpack.obj")
  , m_baseShader(std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.vert",
                 std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.frag")
  , m_normalVisualizationShader(std::string(RESSOURCES_FOLDER) + "/shaders/NormalVis.vert",
                                std::string(RESSOURCES_FOLDER) + "/shaders/NormalVis.frag",
                                std::string(RESSOURCES_FOLDER) + "/shaders/NormalVis.geom")
{
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void NormalVisualizationScene::onEvent(core::Event& event) {}

void NormalVisualizationScene::onUpdate()
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

void NormalVisualizationScene::drawScene()
{
    float t = glfwGetTime();
    float x = cos(t * m_movingSpeedYaw) * m_cameraDistance;
    float z = sin(t * m_movingSpeedYaw) * m_cameraDistance;
    float y = sin(t * m_movingSpeedPitch) * m_cameraAmplitude * 0.5f;
    m_camera.setPosition(glm::vec3(x, y, z));
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));

    m_baseShader.bind();
    m_baseShader.setMat4("projection", m_camera.getProjection());
    m_baseShader.setMat4("view", m_camera.getView());
    m_baseShader.setMat4("model", glm::mat4(1.0f));
    m_model.draw(m_baseShader);

    m_normalVisualizationShader.bind();
    m_normalVisualizationShader.setMat4("projection", m_camera.getProjection());
    m_normalVisualizationShader.setMat4("view", m_camera.getView());
    m_normalVisualizationShader.setMat4("model", glm::mat4(1.0f));
    m_model.draw(m_normalVisualizationShader);
}