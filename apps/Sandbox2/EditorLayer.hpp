#pragma once

#include <SimulatedEntity.hpp>
#include <core/Layer.hpp>
#include <filesystem>
#include <imgui.h>
#include <memory>
#include <renderer/Shader.hpp>
#include <renderer/Shapes/Square.hpp>

namespace fs = std::filesystem;

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

    std::vector<SimulatedEntity> m_simulatedEntites;
    renderer::Square m_square;
    std::unique_ptr<renderer::Shader> m_squareShader;

    // Viewport
    ImVec2 m_vMin{0.0f, 0.0f};
    ImVec2 m_vMax{800.0f, 600.0f};
};
