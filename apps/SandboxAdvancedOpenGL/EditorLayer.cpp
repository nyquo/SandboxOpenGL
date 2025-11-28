#include "EditorLayer.hpp"

#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
{}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    glClearColor(m_windowBackgroundColor.r, m_windowBackgroundColor.g, m_windowBackgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_coloredHouseScene.onUpdate();
    m_exploadingModelScene.onUpdate();
    m_normalVisualizationScene.onUpdate();
    m_instanceRenderingScene.onUpdate();
}

void EditorLayer::onImGuiRender()
{
    m_coloredHouseScene.displayViewportWindow();
    m_exploadingModelScene.displayViewportWindow();
    m_normalVisualizationScene.displayViewportWindow();
    m_instanceRenderingScene.displayViewportWindow();
}

void EditorLayer::onEvent(core::Event& e) { core::EventDispatcher dispatcher(e); }

void EditorLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
    m_coloredHouseScene.setLayerSize(width, height);
    m_exploadingModelScene.setLayerSize(width, height);
    m_normalVisualizationScene.setLayerSize(width, height);
    m_instanceRenderingScene.setLayerSize(width, height);
}
