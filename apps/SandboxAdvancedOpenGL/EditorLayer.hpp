#pragma once

#include "ColoredHouseScene.hpp"

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
    float m_layerWidth;
    float m_layerHeight;

    ColoredHouseScene m_coloredHouseScene{m_layerWidth, m_layerHeight};
    glm::vec3 m_windowBackgroundColor{0.1f, 0.1f, 0.1f};
};