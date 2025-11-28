#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Model.hpp>
#include <renderer/PerspectiveCamera.hpp>
#include <renderer/Shader.hpp>

class InstanceRenderingScene : public core::Viewport
{
  public:
    InstanceRenderingScene(float layerWidth, float layerHeight);
    virtual ~InstanceRenderingScene() = default;

    void onEvent(core::Event& event) override;
    void onUpdate() override;

  private:
    void drawScene();
    void updateCameraPosition();

  private:
    float m_cameraDistance{20.0f};
    float m_cameraHeight{5.0f};
    float m_cameraSpeed{0.2f};

    renderer::Shader m_basicShader;
    renderer::Model m_planetModel;
    renderer::Model m_backpackModel;
    renderer::PerspectiveCamera m_camera;
    renderer::VertexBuffer m_instanceVBO;
};