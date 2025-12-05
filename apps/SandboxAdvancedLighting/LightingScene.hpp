#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
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
    void onUpdate() override;

  private:
    void drawScene();

  private:
    renderer::PerspectiveCamera m_camera;
    std::unique_ptr<renderer::Mesh> m_floorMesh;
    renderer::Shader m_shader;
};