#pragma once

#include "MeshLoadingScene.hpp"

#include <core/Events/WindowEvent.hpp>
#include <core/Layer.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>

struct ViewportData
{
    ImVec2 m_vMin{0.0f, 0.0f};
    ImVec2 m_vMax{800.0f, 600.0f};

    float x{0}, y{0};
    float width{800}, height{600};
};

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
    void displayViewportWindow();
    void displayOptionWindow();
    void optionSingleColorBackground();
    void optionVignetteBackground();
    void optionGradientBackground();
    void optionInfiniteGridBackground();

    bool isCoordInViewport(glm::vec2 coord);

    bool onFileDropped(core::FileDropEvent& event);

  private:
    float m_layerWidth;
    float m_layerHeight;

    MeshLoadingScene m_meshLoadingScene;

    ViewportData m_viewportData;
    glm::vec3 m_windowBackgroundColor{0.1f, 0.1f, 0.1f};
};
