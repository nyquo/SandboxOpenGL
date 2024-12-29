#include "EditorLayer.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
{
    constexpr size_t nbEntities = 1;
    auto& entites = m_scene.getEntites();
    entites.reserve(nbEntities);
    for(int i = 0; i < nbEntities; ++i)
    {
        entites.emplace_back();
    }
}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    glViewport(0,0, m_layerWidth, m_layerHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer.setViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    m_renderer.beginFrame();
    updateSimulatedEntitiesPositions();
    drawSimulatedEntites();
    m_renderer.endFrame();
}

void EditorLayer::onImGuiRender()
{
    float fps = ImGui::GetIO().Framerate;
    ImGui::Begin("Option Window");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Simualted entites: %li", m_scene.getEntites().size());
    ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{1, 1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Begin("Viewport");

    auto& vMin = m_viewportData.m_vMin;
    auto& vMax = m_viewportData.m_vMax;

    vMin = ImGui::GetWindowContentRegionMin();
    vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    m_viewportData.x = vMin.x;
    m_viewportData.y = m_layerHeight - vMax.y;
    m_viewportData.width = vMax.x - vMin.x;
    m_viewportData.height = vMax.y - vMin.y;

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void EditorLayer::onEvent(core::Event& e) { core::EventDispatcher dispatcher(e); }

void EditorLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
}

void EditorLayer::updateSimulatedEntitiesPositions()
{
    // for(auto& entity : m_simulatedEntites) {}
}

void EditorLayer::drawSimulatedEntites() { m_renderer.renderScene(m_scene); }
