#include "InstanceRenderingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
InstanceRenderingScene::InstanceRenderingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Instance Rendering", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_basicShader(std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.vert",
                  std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.frag")
  , m_planetModel(std::string(RESSOURCES_FOLDER) + "/Models/planet/planet.obj")
  , m_backpackModel(std::string(RESSOURCES_FOLDER) + "/Models/backpack/backpack.obj")
  , m_camera(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, m_cameraHeight, m_cameraDistance))
  , m_instanceVBO(sizeof(glm::vec2) * 100, nullptr)
{
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void InstanceRenderingScene::onEvent(core::Event& event) {}

void InstanceRenderingScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    m_camera.setViewPortSize(getWidth(), getHeight());
    updateCameraPosition();

    begin();
    drawScene();
    end();
}

void InstanceRenderingScene::drawScene()
{
    m_basicShader.bind();
    m_basicShader.setMat4("projection", m_camera.getProjection());
    m_basicShader.setMat4("view", m_camera.getView());
    glm::mat4 model = glm::mat4(1.0f);
    m_basicShader.setMat4("model", model);
    m_planetModel.draw(m_basicShader);
    // m_quadShader.bind();
    // m_quadVAO.bind();
    // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}

void InstanceRenderingScene::updateCameraPosition()
{
    float t = glfwGetTime() * m_cameraSpeed;
    float x = cos(t) * m_cameraDistance;
    float z = sin(t) * m_cameraDistance;
    m_camera.setPosition(glm::vec3(x, m_cameraHeight, z));
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}