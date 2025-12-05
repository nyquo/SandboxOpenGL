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
    m_lightingScene.onUpdate();
}

void EditorLayer::onImGuiRender() { m_lightingScene.displayViewportWindow(); }

void EditorLayer::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    m_lightingScene.onEvent(e);
}

void EditorLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
    m_lightingScene.setLayerSize(width, height);
}
