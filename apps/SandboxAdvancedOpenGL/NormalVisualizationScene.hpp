#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Model.hpp>
#include <renderer/PerspectiveCamera.hpp>
#include <renderer/Shader.hpp>

class NormalVisualizationScene : public core::Viewport
{
  public:
    NormalVisualizationScene(float layerWidth, float layerHeight);
    virtual ~NormalVisualizationScene() = default;

    void onEvent(core::Event& event) override;
    void onUpdate() override;

  private:
    void drawScene();

  private:
    float m_cameraDistance{10.0f};
    float m_cameraAmplitude{5.0f};
    float m_movingSpeedYaw{1.0f};
    float m_movingSpeedPitch{2.0f};

    renderer::PerspectiveCamera m_camera;
    renderer::Model m_model;
    renderer::Shader m_baseShader;
    renderer::Shader m_normalVisualizationShader;
};