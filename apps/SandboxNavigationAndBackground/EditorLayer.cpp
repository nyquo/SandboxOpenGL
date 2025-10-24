#include "EditorLayer.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
  , m_camera(std::make_shared<renderer::PerspectiveCamera>(m_layerWidth, m_layerHeight, glm::vec3(0.0F, 0.0F, 50.0F)))
{}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    glClearColor(m_windowBackgroundColor.r, m_windowBackgroundColor.g, m_windowBackgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_meshLoadingScene.setViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    m_camera->setViewPortSize(m_viewportData.width, m_viewportData.height);
    m_meshLoadingScene.renderScene();
}

void EditorLayer::onImGuiRender()
{
    displayOptionWindow();
    displayViewportWindow();
}

void EditorLayer::onEvent(core::Event& e) { core::EventDispatcher dispatcher(e); }

void EditorLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
}

void EditorLayer::displayOptionWindow()
{
    float fps = ImGui::GetIO().Framerate;
    ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Option Window");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::ColorEdit3("Background Color", (float*)&m_meshLoadingScene.getOptions().backgroundColor);
    ImGui::End();
}

void EditorLayer::displayViewportWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{1, 1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
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