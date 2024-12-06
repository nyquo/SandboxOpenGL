#pragma once

#include <SimulatedEntity.hpp>
#include <core/Layer.hpp>
#include <renderer/Shader.hpp>
#include <renderer/Shapes/Square.hpp>

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

class EditorLayer : public core::Layer
{
  public:
    EditorLayer(float width = 0, float height = 0);
    ~EditorLayer();

    void onUpdate() override;
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
};
