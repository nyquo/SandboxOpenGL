#include "CylinderCameraMover.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

CylinderCameraMover::CylinderCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera)
  : CameraMover(camera)
{}

void CylinderCameraMover::update()
{
    auto* currentWindow = glfwGetCurrentContext();
    if(!core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        m_firstMouse = true;
        if(currentWindow != nullptr && m_enabled)
        {
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else
    {
        if(currentWindow != nullptr && m_enabled)
        {
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(!m_camera || !m_enabled)
    {
        return;
    }
}

void CylinderCameraMover::init() { moveCamera(0.0, -15.0); }

void CylinderCameraMover::onEvent(core::Event& event)
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(CylinderCameraMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(CylinderCameraMover::onMouseMoved));
}

void CylinderCameraMover::moveCamera(double horizontalAngleOffset, double verticalOffset)
{
    auto oldCameraPos = m_camera->getPosition();
    double newY = std::clamp(oldCameraPos.y + verticalOffset, m_minHeight, m_maxHeight);
    double newX = oldCameraPos.x;
    double newZ = oldCameraPos.z;

    glm::vec3 newCameraPos = rotateAroundPoint(glm::vec3(newX, newY, newZ),
                                               glm::vec3(0.0F, 0.0F, 0.0F),
                                               glm::radians(static_cast<float>(horizontalAngleOffset)));

    m_camera->setPosition(newCameraPos);
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

glm::vec3 CylinderCameraMover::rotateAroundPoint(const glm::vec3& pointToRotate,
                                                 const glm::vec3& pivot,
                                                 float angleRadians,
                                                 const glm::vec3& axis)
{
    glm::vec3 relative = pointToRotate - pivot;
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, axis);
    glm::vec3 rotatedRelative = glm::vec3(rotationMatrix * glm::vec4(relative, 1.0));
    return pivot + rotatedRelative;
}

bool CylinderCameraMover::onMouseScrolled(core::MouseScrolledEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool CylinderCameraMover::onMouseMoved(core::MouseMovedEvent& event)
{
    if(!m_camera || !m_enabled)
    {
        return false;
    }
    if(core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        if(m_firstMouse)
        {
            m_lastMouseX = event.getX();
            m_lastMouseY = event.getY();
            m_firstMouse = false;
        }
        double xOffset = m_lastMouseX - event.getX();
        double yOffset = event.getY() - m_lastMouseY;
        m_lastMouseX = event.getX();
        m_lastMouseY = event.getY();
        xOffset *= m_mouseSensitivityX;
        yOffset *= m_mouseSensitivityY;

        moveCamera(xOffset, yOffset);
    }

    return false;
}