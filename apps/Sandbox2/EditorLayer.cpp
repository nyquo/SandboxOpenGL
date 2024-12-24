#include "EditorLayer.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
{
    constexpr size_t nbEntities = 1;
    m_simulatedEntites.reserve(nbEntities);
    for(int i = 0; i < nbEntities; ++i)
    {
        m_simulatedEntites.emplace_back();
    }
    m_squareShader = std::make_unique<renderer::Shader>(fs::path(RESSOURCES_FOLDER) / "shaders" / "squareShader.vert",
                                                        fs::path(RESSOURCES_FOLDER) / "shaders" / "squareShader.frag");
}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    glViewport(0,0, m_layerWidth, m_layerHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    glScissor(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    updateSimulatedEntitiesPositions();
    drawSimulatedEntites();
}

void EditorLayer::onImGuiRender()
{
    float fps = ImGui::GetIO().Framerate;
    ImGui::Begin("Option Window");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Simualted entites: %li", m_simulatedEntites.size());
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
    for(auto& entity : m_simulatedEntites) {}
}

void EditorLayer::drawSimulatedEntites()
{
    m_squareShader->bind();
    m_square.getIndexBuffer().bind();
    m_square.getVertexArray().bind();
    for(auto& entity : m_simulatedEntites)
    {
        m_squareShader->setVec3("squareColor", entity.m_color);
        glDrawElements(GL_TRIANGLES, m_square.getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}
