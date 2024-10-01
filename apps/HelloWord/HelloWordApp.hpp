#pragma once

#include "HappyCube.hpp"

#include <Application.hpp>
#include <Events/Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/WindowEvent.hpp>
#include <Layer.hpp>
#include <Logger.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CustomLayer : public core::Layer
{
  public:
    CustomLayer();

    ~CustomLayer();

    void onUpdate() override;

    void onEvent(core::Event& e) override
    {
        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(CustomLayer::onMouseScrolled));
        dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(CustomLayer::onMouseMoved));
        dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(CustomLayer::onKeyPressed));
        dispatcher.dispatch<core::KeyReleasedEvent>(BIND_EVENT_FN(CustomLayer::onKeyReleased));
    }
    int m_windowWidth{800};
    int m_windowHeight{600};

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

    std::unique_ptr<core::Shader> shaderProgram;
    std::unique_ptr<core::Texture> texture1, texture2;

    float fov{45};
    float step{2};

    float m_lastMouseX = m_windowWidth / 2;
    float m_lastMouseY = m_windowHeight / 2;
    const float sensitivity = 0.1f;

    float m_yaw{-90.0};
    float m_pitch{0.0};

    float m_deltaTime{0.0};
    float m_lastFrame{0.0};

    bool m_firstMouse{true};

    glm::vec3 m_cameraPos{0.0f, 0.0f, 10.0f};
    glm::vec3 m_cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};
};

class HelloWordApp : public core::Application
{
  public:
    HelloWordApp()
    {
        getWindow().setLayer(std::make_shared<CustomLayer>());

        auto customLayer = std::dynamic_pointer_cast<CustomLayer>(getWindow().getLayer());

        customLayer->m_windowHeight = getWindow().getHeight();
        customLayer->m_windowWidth = getWindow().getWidth();
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(HelloWordApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        auto customLayer = std::dynamic_pointer_cast<CustomLayer>(getWindow().getLayer());

        customLayer->m_windowHeight = e.getHeight();
        customLayer->m_windowWidth = e.getWidth();

        return false;
    }
};
