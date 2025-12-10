#include "LightingScene.hpp"

#include "imgui.h"

#include <core/gl.h>

LightingScene::LightingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Lighting Scene", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_camera(
      std::make_shared<renderer::PerspectiveCamera>(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, 0.0F, 10.0F)))
  , m_shader(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag")
  , m_lightCubeShader(std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.vert",
                      std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.frag",
                      std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.geom")
  , m_cameraMover(m_camera)
{
    const float floorHalfSize = 10.0f;
    std::vector<renderer::Vertex> floorVertices = {
      // positions            // normals         // texture coords
      {{floorHalfSize, 0.0f, floorHalfSize}, {0.0f, 1.0f, 0.0f}, {10.0f, 10.0f}},
      {{-floorHalfSize, 0.0f, -floorHalfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{-floorHalfSize, 0.0f, floorHalfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
      {{floorHalfSize, 0.0f, -floorHalfSize}, {0.0f, 1.0f, 0.0f}, {10.0f, 0.0f}},
    };

    std::vector<unsigned int> floorIndices = {
      0,
      1,
      2,
      0,
      3,
      1,
    };

    std::vector<renderer::Texture> floorTextures;
    floorTextures.emplace_back(std::string(RESSOURCES_FOLDER) + "/textures/wood.png", "texture_diffuse");

    m_floorMesh =
      std::make_unique<renderer::Mesh>(std::move(floorVertices), std::move(floorIndices), std::move(floorTextures));

    m_cameraMover.init();

    // Setup point light VBO and VAO
    updatePointLightBuffer();
    renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                                  renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float))};
    m_pointLightsVBO.setLayout(std::move(layout));
    m_pointLightsVAO.addVertexBuffer(m_pointLightsVBO);
}

void LightingScene::onEvent(core::Event& event) { m_cameraMover.onEvent(event); }

void LightingScene::onImGuiRender()
{
    ImGui::Begin("Lighting Scene Settings");
    ImGui::ColorEdit3("Point light color", (float*)&m_pointLight.m_diffuseColor);
    ImGui::DragFloat3("Point light position", (float*)&m_pointLight.m_position, 0.01f);
    ImGui::End();
}

void LightingScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    m_camera->setViewPortSize(getWidth(), getHeight());
    m_cameraMover.update();

    begin();
    drawScene();
    end();
}

void LightingScene::drawScene()
{
    m_shader.bind();
    m_shader.setMat4("projection", m_camera->getProjection());
    m_shader.setMat4("view", m_camera->getView());
    m_shader.setMat4("model", glm::mat4(1.0f));
    m_floorMesh->draw(m_shader);

    updatePointLightBuffer();
    m_lightCubeShader.bind();
    m_lightCubeShader.setMat4("projection", m_camera->getProjection());
    m_lightCubeShader.setMat4("view", m_camera->getView());
    m_pointLightsVAO.bind();
    glDrawArrays(GL_POINTS, 0, 1);
}

void LightingScene::updatePointLightBuffer()
{
    float pointLightData[6] = {
      m_pointLight.m_position.x,
      m_pointLight.m_position.y,
      m_pointLight.m_position.z,
      m_pointLight.m_diffuseColor.r,
      m_pointLight.m_diffuseColor.g,
      m_pointLight.m_diffuseColor.b,
    };
    m_pointLightsVBO.setData(pointLightData, sizeof(pointLightData));
}