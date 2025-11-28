#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
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

  private:
    renderer::Shader m_quadShader;
    // renderer::VertexArray m_quadVAO;
    // renderer::VertexBuffer m_quadVBO;
};