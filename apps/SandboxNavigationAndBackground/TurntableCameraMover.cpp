#include "TurntableCameraMover.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

TurntableCameraMover::TurntableCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera)
  : CameraMover(camera)
{}

void TurntableCameraMover::update()
{
    if(!m_camera || !m_enabled)
    {
        return;
    }

    float deltaYaw = m_currentMousePos.x - m_lastMousePos.x;
    float deltaPitch = m_currentMousePos.y - m_lastMousePos.y;
    m_lastMousePos = m_currentMousePos;

    m_yaw -= glm::radians(deltaYaw * m_rotationSpeedX);
    m_pitch += glm::radians(deltaPitch * m_rotationSpeedY);
    m_pitch = std::clamp(m_pitch, m_minPitch, m_maxPitch);

    float x = distance * cosf(m_pitch) * sinf(m_yaw);
    float y = distance * sinf(m_pitch);
    float z = distance * cosf(m_pitch) * cosf(m_yaw);

    glm::vec3 pos = glm::vec3(x, y, z);
    m_camera->setPosition(pos);
    m_camera->lookAt(m_target);
}

void TurntableCameraMover::init()
{
    m_camera->setPosition(glm::vec3(0.0F, 0.0F, -distance));
    m_camera->lookAt(m_target);
}

void TurntableCameraMover::onEvent(core::Event& event)
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(TurntableCameraMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(TurntableCameraMover::onMouseMoved));
    dispatcher.dispatch<core::MouseButtonPressedEvent>(BIND_EVENT_FN(TurntableCameraMover::onMouseButtonPressed));
    dispatcher.dispatch<core::MouseButtonReleasedEvent>(BIND_EVENT_FN(TurntableCameraMover::onMouseButtonReleased));
}

bool TurntableCameraMover::onMouseScrolled(core::MouseScrolledEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool TurntableCameraMover::onMouseMoved(core::MouseMovedEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    if(core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        m_currentMousePos = core::Input::getMousePosition();
    }

    return false;
}

bool TurntableCameraMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_lastMousePos = core::Input::getMousePosition();
        m_currentMousePos = core::Input::getMousePosition();
    }
    return false;
}

bool TurntableCameraMover::onMouseButtonReleased(core::MouseButtonReleasedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}
