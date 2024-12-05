#include "FpsCameraMover.hpp"

#include <core/gl.h>

namespace renderer {

FpsCameraMover::FpsCameraMover(const std::shared_ptr<PerspectiveCamera>& camera)
  : m_camera(camera)
{}

void FpsCameraMover::update()
{
    double currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    if(!m_camera)
    {
        return;
    }

    if(!m_enabled)
    {
        return;
    }

    const float cameraSpeed = 10.0F * m_deltaTime;
    glm::vec3 positionOffset{0.0F, 0.0F, 0.0F};

    if(core::Input::isKeyPressed(GLFW_KEY_W))
    {
        positionOffset.z += cameraSpeed;
    }
    if(core::Input::isKeyPressed(GLFW_KEY_S))
    {
        positionOffset.z -= cameraSpeed;
    }
    if(core::Input::isKeyPressed(GLFW_KEY_A))
    {
        positionOffset.x -= cameraSpeed;
    }
    if(core::Input::isKeyPressed(GLFW_KEY_D))
    {
        positionOffset.x += cameraSpeed;
    }
    if(core::Input::isKeyPressed(GLFW_KEY_SPACE))
    {
        positionOffset.y += cameraSpeed;
    }
    if(core::Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        positionOffset.y -= cameraSpeed;
    }
    m_camera->translateCameraRelative(positionOffset);
}

void FpsCameraMover::setCamera(const std::shared_ptr<PerspectiveCamera>& camera) { m_camera = camera; }

void FpsCameraMover::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(FpsCameraMover::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(FpsCameraMover::onMouseMoved));
}

void FpsCameraMover::enable() { m_enabled = true; }

void FpsCameraMover::disable() { m_enabled = false; }

bool FpsCameraMover::onMouseScrolled(core::MouseScrolledEvent& e)
{
    if(m_enabled)
    {
        m_camera->adustFov(-m_zoomOffset * e.getYOffset());
    }
    return true;
}

bool FpsCameraMover::onMouseMoved(core::MouseMovedEvent& e)
{
    if(m_firstMouse)
    {
        m_lastMouseX = e.getX();
        m_lastMouseY = e.getY();
        m_firstMouse = false;
    }
    float xOffset = e.getX() - m_lastMouseX;
    float yOffset = m_lastMouseY - e.getY();
    m_lastMouseX = e.getX();
    m_lastMouseY = e.getY();
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    if(m_enabled)
    {
        m_camera->rotateCamera(xOffset, yOffset);
    }

    return true;
}

}
