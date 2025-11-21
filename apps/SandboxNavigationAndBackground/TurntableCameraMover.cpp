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

    glm::vec2 deltaMouse = m_lastMousePos - m_firstMousePos;
    float angleY = glm::radians(deltaMouse.x * m_mouseSensitivityX);
    float angleX = glm::radians(deltaMouse.y * m_mouseSensitivityY);
    glm::vec3 eulerAngles(-angleX, -angleY, 0.0F);
    glm::quat cameraRot(eulerAngles);
    glm::mat4 rotationMatrix = glm::mat4_cast(cameraRot);
    glm::vec3 newCamPos = rotationMatrix * glm::vec4(m_firstCamPos, 1.0F);
    m_camera->setPosition(newCamPos);
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

void TurntableCameraMover::init()
{
    m_camera->setPosition(m_originalPosition);
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
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
        m_lastMousePos = core::Input::getMousePosition();
    }

    return false;
}

bool TurntableCameraMover::onMouseButtonPressed(core::MouseButtonPressedEvent& event)
{
    auto* currentWindow = glfwGetCurrentContext();
    if(event.getButtonCode() == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_firstMousePos = core::Input::getMousePosition();
        m_firstCamPos = m_camera->getPosition();
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
