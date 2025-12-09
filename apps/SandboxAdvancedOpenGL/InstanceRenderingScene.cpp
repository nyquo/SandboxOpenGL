#include "InstanceRenderingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
#include <glm/gtc/matrix_transform.hpp>

InstanceRenderingScene::InstanceRenderingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Instance Rendering", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_basicShader(std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.vert",
                  std::string(RESSOURCES_FOLDER) + "/shaders/BaseModel.frag")
  , m_instanceShader(std::string(RESSOURCES_FOLDER) + "/shaders/InstanceModel.vert",
                     std::string(RESSOURCES_FOLDER) + "/shaders/InstanceModel.frag")
  , m_planetModel(std::string(RESSOURCES_FOLDER) + "/Models/planet/planet.obj")
  , m_backpackModel(std::string(RESSOURCES_FOLDER) + "/Models/backpack/backpack.obj")
  , m_camera(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, m_cameraHeight, m_cameraDistance))
  , m_instancesVBO(sizeof(glm::mat4) * m_amountOfInstances, nullptr)
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

    m_instancesVBO.bind();
    m_instancesVBO.setData(m_instancesModelMatrices.data(), sizeof(glm::mat4) * m_amountOfInstances);

    for(auto& mesh : m_backpackModel.getMeshes()) // TEMP
    {
        mesh.getVAO().bind();
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
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

    m_instanceShader.bind();
    m_instanceShader.setMat4("projection", m_camera.getProjection());
    m_instanceShader.setMat4("view", m_camera.getView());
    for(auto& mesh : m_backpackModel.getMeshes())
    {
        mesh.getVAO().bind();
        glDrawElementsInstanced(GL_TRIANGLES,
                                static_cast<GLsizei>(mesh.indices.size()),
                                GL_UNSIGNED_INT,
                                0,
                                static_cast<GLsizei>(m_amountOfInstances));
    }
}

void InstanceRenderingScene::updateCameraPosition()
{
    float t = glfwGetTime() * m_cameraSpeed;
    float x = cos(t) * m_cameraDistance;
    float z = sin(t) * m_cameraDistance;
    m_camera.setPosition(glm::vec3(x, m_cameraHeight, z));
    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}