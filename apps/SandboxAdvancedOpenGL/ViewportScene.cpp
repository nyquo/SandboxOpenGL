#include "ViewportScene.hpp"

#include <core/gl.h>

void ViewportScene::onEvent(core::Event& event) {}

void ViewportScene::onUpdate()
{
    begin();
    // Rendering code would go here
    end();
}

void ViewportScene::setViewport(float x, float y, float width, float height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

void ViewportScene::begin()
{
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_x, m_y, m_width, m_height);
    glScissor(m_x, m_y, m_width, m_height);
    glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ViewportScene::end() { glDisable(GL_SCISSOR_TEST); }