#include "InstanceRenderingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
#include <glm/gtc/matrix_transform.hpp>

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

    m_instancesModelMatrices.reserve(m_amountOfInstances);

    srand(glfwGetTime()); // initialize random seed
    const float radius = 30.0;
    const float offset = 2.5f;

    for(int i = 0; i < m_amountOfInstances; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);

        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)m_amountOfInstances * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        m_instancesModelMatrices.push_back(model);
    }
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

    for(const auto& instanceModelMat : m_instancesModelMatrices)
    {
        m_basicShader.setMat4("model", instanceModelMat);
        m_backpackModel.draw(m_basicShader);
    }
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