#include "Layer.hpp"

#include "Logger.hpp"

#include <Window.hpp>

namespace core {

Layer::Layer(Window* window)
  : m_window(window)
{}

bool Layer::isKeyPressed(int key) const
{
    if(m_window)
    {
        return m_window->isKeyPressed(key);
    }

    core::Logger::logWarning("isKeyPressed called on a Layer not attached to any window!");
    return false;
}

bool Layer::isMouseButtonPressed(int button) const
{
    if(m_window)
    {
        return m_window->isMouseButtonPressed(button);
    }

    core::Logger::logWarning("isMouseButtonPressed called on a Layer not attached to any window!");
    return false;
}

glm::vec2 Layer::getMousePosition() const
{
    if(m_window)
    {
        return m_window->getMousePosition();
    }

    core::Logger::logWarning("getMousePosition called on a Layer not attached to any window!");
    return {0, 0};
}

void Layer::setWindow(Window* window) { m_window = window; }

}
