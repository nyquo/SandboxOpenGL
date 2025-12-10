#include "TrackballCameraMover.hpp"

#include <algorithm>
#include <core/Input.hpp>
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

    if(m_moveCameraMode)
    {
        rotateCamera();
    }
    else
    {
        moveCamera();
    }
}

void TrackballCameraMover::init()
{
    m_pitch = glm::radians(20.0f);
    m_yaw = glm::radians(0.0f);
    rotateCamera();
    updateCameraDistance();
}

void TrackballCameraMover::rotateCamera()
{
    float deltaYaw = m_currentMousePos.x - m_lastMousePos.x;
    float deltaPitch = m_currentMousePos.y - m_lastMousePos.y;
    m_lastMousePos = m_currentMousePos;

    m_yaw -= glm::radians(deltaYaw * m_mouseSensitivityX);
    m_pitch += glm::radians(deltaPitch * m_mouseSensitivityY);
    m_pitch = std::clamp(m_pitch, m_minPitch, m_maxPitch);

    updateCameraPosition();
}

void TrackballCameraMover::moveCamera()
{
    float deltaX = m_currentMousePos.x - m_lastMousePos.x;
    float deltaY = m_currentMousePos.y - m_lastMousePos.y;
    m_lastMousePos = m_currentMousePos;

    auto cameraRight = m_camera->getRight();
    auto cameraUp = m_camera->getUp();

    m_target -= deltaX * 0.005f * m_mouseSensitivityX * m_distance * cameraRight;
    m_target += deltaY * 0.005f * m_mouseSensitivityY * m_distance * cameraUp;

    updateCameraPosition();
}

void TrackballCameraMover::updateCameraPosition()
{
    float x = m_distance * cosf(m_pitch) * sinf(m_yaw);
    float y = m_distance * sinf(m_pitch);
    float z = m_distance * cosf(m_pitch) * cosf(m_yaw);

    glm::vec3 pos = glm::vec3(x, y, z);
    m_camera->setPosition(pos + m_target);
    m_camera->lookAt(m_target);
}

void TrackballCameraMover::updateCameraDistance()
{
    m_distance = pow(1.1f, m_distanceMultiplier);
    updateCameraPosition();
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
    }

    m_distanceMultiplier += -event.getYOffset();
    m_distanceMultiplier = std::max(0.0f, m_distanceMultiplier);
    updateCameraDistance();

    return false;
}

bool TrackballCameraMover::onMouseMoved(core::MouseMovedEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    auto buttonChecked = m_touchScreenMode ? GLFW_MOUSE_BUTTON_LEFT : GLFW_MOUSE_BUTTON_MIDDLE;
    if(core::Input::isMouseButtonPressed(buttonChecked))
    {
        m_currentMousePos = core::Input::getMousePosition();
    }

    return false;
}

bool TrackballCameraMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    auto buttonChecked = m_touchScreenMode ? GLFW_MOUSE_BUTTON_LEFT : GLFW_MOUSE_BUTTON_MIDDLE;
    if(event.getButtonCode() == buttonChecked)
    {
        if(!m_touchScreenMode)
        {
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        m_lastMousePos = core::Input::getMousePosition();
        m_currentMousePos = core::Input::getMousePosition();
        m_moveCameraMode = !core::Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT);
    }
    return false;
}

bool TrackballCameraMover::onMouseButtonReleased(core::MouseButtonReleasedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    auto buttonChecked = m_touchScreenMode ? GLFW_MOUSE_BUTTON_LEFT : GLFW_MOUSE_BUTTON_MIDDLE;
    if(event.getButtonCode() == buttonChecked && !m_touchScreenMode)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}
