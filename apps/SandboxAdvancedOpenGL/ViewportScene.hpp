#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Shader.hpp>

class ViewportScene : public core::Viewport
{
  public:
    ViewportScene(float layerWidth, float layerHeight);
    virtual ~ViewportScene() = default;

    void onEvent(core::Event& event) override;
    void onUpdate() override;

  private:
  private:
    std::unique_ptr<renderer::Shader> m_geomTrialShader;
    renderer::VertexBuffer m_vbo{20 * sizeof(float)};
    renderer::VertexArray m_vao;
};