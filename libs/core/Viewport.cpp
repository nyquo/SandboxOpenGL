#include "Viewport.hpp"

#include <core/gl.h>

namespace core {

Viewport::Viewport(float m_layerWidth,
                   float m_layerHeight,
                   std::string viewportName,
                   float x,
                   float y,
                   float width,
                   float height,
                   glm::vec3 backgroundColor)
  : m_layerWidth(m_layerWidth)
  , m_layerHeight(m_layerHeight)
  , m_viewportName(viewportName)
  , m_x(x)
  , m_y(y)
  , m_width(width)
  , m_height(height)
  , m_backgroundColor(backgroundColor)
{}

Viewport::~Viewport() {}

float Viewport::getX() const { return m_x; }
float Viewport::getY() const { return m_y; }
float Viewport::getWidth() const { return m_width; }
float Viewport::getHeight() const { return m_height; }

void Viewport::begin()
{
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_x, m_y, m_width, m_height);
    glScissor(m_x, m_y, m_width, m_height);
    glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Viewport::end() { glDisable(GL_SCISSOR_TEST); }

void Viewport::displayViewportWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{1, 1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::SetNextWindowPos(ImVec2(m_x, m_y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
    ImGui::Begin(m_viewportName.c_str());

    auto& vMin = m_vMin;
    auto& vMax = m_vMax;

    vMin = ImGui::GetWindowContentRegionMin();
    vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    m_isCollapsed = ImGui::IsWindowCollapsed();

    m_x = vMin.x;
    m_y = m_layerHeight - vMax.y;
    m_width = vMax.x - vMin.x;
    m_height = vMax.y - vMin.y;

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void Viewport::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
}

}