#pragma once

#include "ViewportScene.hpp"

#include <core/Layer.hpp>
#include <core/Viewport.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

class EditorLayer : public core::Layer
{
  public:
    EditorLayer(float width = 0, float height = 0);
    ~EditorLayer();

    void onUpdate() override;
    void onImGuiRender() override;
    void onEvent(core::Event& e) override;

    void setLayerSize(float width, float height);

  private:
    void updateViewportSceneBounds();

  private:
    float m_layerWidth;
    float m_layerHeight;

    core::Viewport m_viewport{m_layerWidth, m_layerHeight, "Viewport", 50, 50, 800, 600};
    ViewportScene m_viewportScene;
    glm::vec3 m_windowBackgroundColor{0.1f, 0.1f, 0.1f};
};