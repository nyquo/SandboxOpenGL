#pragma once

#include <core/Events/Event.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Shader.hpp>

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
};