#include "LightingScene.hpp"

#include <core/gl.h>

LightingScene::LightingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Lighting Scene", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_camera(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, 2.0F, 10.0F))
  , m_shader(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag")
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

    m_camera.lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void LightingScene::onEvent(core::Event& event) {}

void LightingScene::onUpdate()
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

void LightingScene::drawScene()
{
    m_shader.bind();
    m_shader.setMat4("projection", m_camera.getProjection());
    m_shader.setMat4("view", m_camera.getView());
    m_shader.setMat4("model", glm::mat4(1.0f));
    m_floorMesh->draw(m_shader);
}