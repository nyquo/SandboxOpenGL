#pragma once

#include "OffScreenRenderer.hpp"
#include "TrackballCameraMover.hpp"
#include "glm/fwd.hpp"
#include "renderer/FrameBuffer.hpp"

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <random>
#include <renderer/Buffers.hpp>
#include <renderer/Lights.hpp>
#include <renderer/Model.hpp>
#include <renderer/PerspectiveCamera.hpp>
#include <renderer/Shader.hpp>
#include <renderer/Texture.hpp>

class LightingScene : public core::Viewport
{
  public:
    LightingScene(float layerWidth, float layerHeight);
    virtual ~LightingScene() = default;

    void onEvent(core::Event& event) override;
    void onImGuiRender();
    void onUpdate() override;

  private:
    void initFloorMesh();
    void drawScene();
    void updatePointLightBuffer();
    void randomizeModelsPositions();

  private:
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;
    std::unique_ptr<renderer::Mesh> m_floorMesh;
    std::unique_ptr<renderer::Model> m_model;
    std::vector<glm::mat4> m_modelMatrices;
    renderer::Shader m_shader;
    renderer::Shader m_lightCubeShader;
    OffScreenRenderer m_offScreenRenderer;
    TrackballCameraMover m_cameraMover;
    bool m_blinnPhong{true};
    float m_gammaCorrection{2.2};
    renderer::FrameBuffer m_shadowsFB;

    // Lights
    renderer::DirectionalLight m_directionalLight{
      glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.0f), glm::vec3(0.0f)};
    renderer::PointLight m_pointLight{glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(0.3f)};
    renderer::VertexBuffer m_pointLightsVBO{6 * sizeof(float)}; // 3 for position, 3 for color
    renderer::VertexArray m_pointLightsVAO;

    // Random generator
    std::random_device m_rd{};
    std::mt19937 m_gen{m_rd()};
};