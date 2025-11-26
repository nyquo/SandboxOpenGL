#include "EditorLayer.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
  , m_camera(std::make_shared<renderer::PerspectiveCamera>(m_layerWidth, m_layerHeight, glm::vec3(0.0F, 0.0F, 50.0F)))
{
    std::random_device rd;
    m_gen = std::mt19937(rd());

    m_scene.setActiveCamera(m_camera);

    auto& entites = m_scene.getEntites();
    entites.reserve(m_maxEntityCount);
    for(int i = 0; i < m_entityCount; ++i)
    {
        entites.emplace_back();
    }
}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    double currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer.setViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    m_camera->setViewPortSize(m_viewportData.width, m_viewportData.height);
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
    ImGui::DragInt("simulated entities", &m_entityCount, 100, 0, m_maxEntityCount);
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

void EditorLayer::updateSimulatedEntitiesPositions() // todo rename
{
    auto& entities = m_scene.getEntites();
    if(m_entityCount > entities.size())
    {
        const size_t addCount = m_entityCount - entities.size();
        for(int i = 0; i < addCount; ++i)
        {
            entities.emplace_back();
        }
    }
    if(m_entityCount < entities.size())
    {
        entities.erase(entities.end() - (entities.size() - m_entityCount), entities.end());
    }

    const float incColor = 5.0F * m_deltaTime;
    std::uniform_real_distribution<float> distColor(-incColor, incColor);
    const float incPos = 5.0F * m_deltaTime;
    std::uniform_real_distribution<float> distPos(-incPos, incPos);
    for(auto& entity : entities)
    {
        float r = distColor(m_gen);
        float g = distColor(m_gen);
        float b = distColor(m_gen);
        entity.m_color.r = std::clamp(entity.m_color.r + r, 0.0F, 1.0F);
        entity.m_color.g = std::clamp(entity.m_color.g + g, 0.0F, 1.0F);
        entity.m_color.b = std::clamp(entity.m_color.b + b, 0.0F, 1.0F);

        float x = distPos(m_gen);
        float y = distPos(m_gen);
        constexpr float bounds = 40.0F;
        entity.m_position.x = std::clamp(entity.m_position.x + x, -bounds, bounds);
        entity.m_position.y = std::clamp(entity.m_position.y + y, -bounds, bounds);
    }
}

void EditorLayer::drawSimulatedEntites() { m_renderer.renderScene(m_scene); }
