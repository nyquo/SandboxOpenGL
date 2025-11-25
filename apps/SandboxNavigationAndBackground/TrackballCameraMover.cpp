#include "TrackballCameraMover.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

TrackballCameraMover::TrackballCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera)
  : CameraMover(camera)
{}

void TrackballCameraMover::update()
{
    if(!m_camera || !m_enabled)
    {
        return;
    }

    float deltaYaw = m_currentMousePos.x - m_lastMousePos.x;
    float deltaPitch = m_currentMousePos.y - m_lastMousePos.y;
    m_lastMousePos = m_currentMousePos;

    m_yaw -= glm::radians(deltaYaw * m_mouseSensitivityX);
    m_pitch += glm::radians(deltaPitch * m_mouseSensitivityY);
    m_pitch = std::clamp(m_pitch, m_minPitch, m_maxPitch);

    float x = distance * cosf(m_pitch) * sinf(m_yaw);
    float y = distance * sinf(m_pitch);
    float z = distance * cosf(m_pitch) * cosf(m_yaw);

    glm::vec3 pos = glm::vec3(x, y, z);
    m_camera->setPosition(pos);
    m_camera->lookAt(m_target);
}

void TrackballCameraMover::init()
{
    m_camera->setPosition(glm::vec3(0.0F, 0.0F, -distance));
    m_camera->lookAt(m_target);
}

void TrackballCameraMover::onEvent(core::Event& event)
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(TrackballCameraMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(TrackballCameraMover::onMouseMoved));
    dispatcher.dispatch<core::MouseButtonPressedEvent>(BIND_EVENT_FN(TrackballCameraMover::onMouseButtonPressed));
    dispatcher.dispatch<core::MouseButtonReleasedEvent>(BIND_EVENT_FN(TrackballCameraMover::onMouseButtonReleased));
}

bool TrackballCameraMover::onMouseScrolled(core::MouseScrolledEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;

        m_camera->adustFov(-2.0 * event.getYOffset());
    }
    // m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool TrackballCameraMover::onMouseMoved(core::MouseMovedEvent& event)
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

bool TrackballCameraMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_lastMousePos = core::Input::getMousePosition();
    }
    return false;
}

bool TrackballCameraMover::onMouseButtonReleased(core::MouseButtonReleasedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}
