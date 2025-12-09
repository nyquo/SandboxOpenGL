#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Shader.hpp>

class ColoredHouseScene : public core::Viewport
{
  public:
    ColoredHouseScene(float layerWidth, float layerHeight);
    virtual ~ColoredHouseScene() = default;

    void onEvent(core::Event& event) override;
    void onUpdate() override;

  private:
    void drawScene();

  private:
    std::unique_ptr<renderer::Shader> m_geomTrialShader;
    renderer::VertexBuffer m_vbo{20 * sizeof(float)};
    renderer::VertexArray m_vao;
};