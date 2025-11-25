#include "OldModelMover.hpp"

#include <algorithm>
#include <core/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

void OldModelMover::update()
{
    if(!m_camera || !m_modelMatrix || !m_enabled)
    {
        return;
    }

    float deltaYaw = m_currentMousePos.x - m_lastMousePos.x;
    float deltaPitch = m_currentMousePos.y - m_lastMousePos.y;
    m_lastMousePos = m_currentMousePos;

    // Create quaternions for each rotation axis
    glm::quat quatYaw = glm::angleAxis(glm::radians(deltaYaw * m_rotationSpeedYaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat quatPitch = glm::angleAxis(glm::radians(-deltaPitch * m_rotationSpeedPitch), glm::vec3(1.0f, 0.0f, 0.0f));

    // Combine quaternions (order matters for proper rotation)
    glm::quat combinedQuat = quatYaw * quatPitch;

    // Convert combined quaternion to rotation matrix
    glm::mat4 rotationMatrix = glm::mat4_cast(combinedQuat);

    // Apply rotation to model matrix
    *m_modelMatrix = rotationMatrix * (*m_modelMatrix);
}

void OldModelMover::init()
{
    if(!m_camera || !m_modelMatrix || !m_enabled)
    {
        return;
    }
    m_camera->setPosition(glm::vec3(0.0F, 0.0F, -m_distance));
    m_camera->lookAt(m_target);
}

void OldModelMover::onEvent(core::Event& event)
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(OldModelMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(OldModelMover::onMouseMoved));
    dispatcher.dispatch<core::MouseButtonPressedEvent>(BIND_EVENT_FN(OldModelMover::onMouseButtonPressed));
    dispatcher.dispatch<core::MouseButtonReleasedEvent>(BIND_EVENT_FN(OldModelMover::onMouseButtonReleased));
}

bool OldModelMover::onMouseScrolled(core::MouseScrolledEvent& event)
{
    if(!m_camera || !m_modelMatrix || !m_enabled)
    {
        return false;
    }
    m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool OldModelMover::onMouseMoved(core::MouseMovedEvent& event)
{
    if(!m_camera || !m_modelMatrix || !m_enabled)
    {
        return false;
    }
    if(core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        m_currentMousePos = core::Input::getMousePosition();
    }

    return false;
}

bool OldModelMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
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

bool OldModelMover::onMouseButtonReleased(core::MouseButtonReleasedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}
