#include "ArcballCameraMover.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>

ArcballCameraMover::ArcballCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera)
  : CameraMover(camera)
{}

void ArcballCameraMover::update()
{
    if(!m_camera || !m_enabled)
    {
        return;
    }

    glm::vec2 deltaMouse = m_lastMousePos - m_firstMousePos;
    float angleX = glm::radians(deltaMouse.x * m_mouseSensitivityX);
    float angleY = glm::radians(deltaMouse.y * m_mouseSensitivityY);
    core::Logger::logInfo("Delta Mouse: ", angleX, ", ", angleY);
}

void ArcballCameraMover::init()
{
    m_camera->setPosition(m_originalPosition);
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void ArcballCameraMover::onEvent(core::Event& event)
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(ArcballCameraMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(ArcballCameraMover::onMouseMoved));
    dispatcher.dispatch<core::MouseButtonPressedEvent>(BIND_EVENT_FN(ArcballCameraMover::onMouseButtonPressed));
    dispatcher.dispatch<core::MouseButtonReleasedEvent>(BIND_EVENT_FN(ArcballCameraMover::onMouseButtonReleased));
}

bool ArcballCameraMover::onMouseScrolled(core::MouseScrolledEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool ArcballCameraMover::onMouseMoved(core::MouseMovedEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    if(core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        m_lastMousePos = core::Input::getMousePosition();
    }

    return false;
}

bool ArcballCameraMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_firstMousePos = core::Input::getMousePosition();
    }
    return false;
}

bool ArcballCameraMover::onMouseButtonReleased(core::MouseButtonReleasedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}
