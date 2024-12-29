#pragma once

#include <core/Layer.hpp>
#include <imgui.h>
#include <memory>
#include <random>
#include <renderer/PerspectiveCamera.hpp>
#include <renderer/RendererV2.hpp>
#include <renderer/SceneV2.hpp>
#include <renderer/Shader.hpp>
#include <renderer/Shapes/Square.hpp>

namespace fs = std::filesystem;

struct ViewportData {
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

    void updateSimulatedEntitiesPositions();
    void drawSimulatedEntites();

  private:
    float m_layerWidth;
    float m_layerHeight;

    renderer::SceneV2 m_scene;
    renderer::RendererV2 m_renderer;
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;

    ViewportData m_viewportData;

    std::mt19937 m_gen;
    double m_deltaTime{0.0};
    double m_lastFrame{0.0};
};
