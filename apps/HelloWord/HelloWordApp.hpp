#pragma once

#include "HappyCube.hpp"

#include <Application.hpp>
#include <Events/Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/WindowEvent.hpp>
#include <Layer.hpp>
#include <Logger.hpp>
#include <PerspectiveCamera.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CustomLayer : public core::Layer
{
  public:
    CustomLayer(float viewportWidth = 0, float viewportHeight = 0);
    ~CustomLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;

    void setViewportSize(float viewportWidth, float viewportHeight);

  private:
    void processInputs();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e);
    bool onMouseMoved(core::MouseMovedEvent& e);
    bool onKeyPressed(core::KeyPressedEvent& e);
    bool onKeyReleased(core::KeyReleasedEvent& e);

  private:
    std::vector<HappyCube> m_happyCubes;
    unsigned int VBO, VAO;

    std::unique_ptr<renderer::Shader> shaderProgram;
    std::unique_ptr<renderer::Texture> texture1, texture2;

    float m_viewportWidth;
    float m_viewportHeight;

    renderer::PerspectiveCamera m_camera;
    float m_zoomOffset{2.0};
    float m_mouseSensitivity{0.1F};
    bool m_firstMouse{true};
    float m_lastMouseX;
    float m_lastMouseY;
    float m_movementSpeed = 10.0F;
    float m_deltaTime{0.0};
    float m_lastFrame{0.0};
};

class HelloWordApp : public core::Application
{
  public:
    HelloWordApp()
      : m_customLayer(std::make_shared<CustomLayer>(getWindow().getWidth(), getWindow().getHeight()))
    {
        glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        getWindow().pushLayer(m_customLayer);
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(HelloWordApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        if(m_customLayer)
        {
            m_customLayer->setViewportSize(e.getWidth(), e.getHeight());
        }
        return false;
    }

  private:
    std::shared_ptr<CustomLayer> m_customLayer;
};
