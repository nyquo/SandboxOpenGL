#pragma once

#include "HappyCube.hpp"

#include <Application.hpp>
#include <Events/Event.hpp>
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
    }
    int m_windowWidth;
    int m_windowHeight;

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e)
    {
        fov = std::max(0.0f, std::min(90.0f, fov + (step * e.getYOffset())));
        return false;
    }
    bool onMouseMoved(core::MouseMovedEvent& e)
    {
        m_mousePosXNorm = ((e.getX() * 2) / m_windowWidth) - 1;
        m_mousePosYNorm = -((e.getY() * 2) / m_windowHeight) + 1;
        return false;
    }

  private:
    std::vector<HappyCube> m_happyCubes;
    /*{glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(2.0f, 5.0f, -15.0f),
                                        glm::vec3(-1.5f, -2.2f, -2.5f),
                                        glm::vec3(-3.8f, -2.0f, -12.3f),
                                        glm::vec3(2.4f, -0.4f, -3.5f),
                                        glm::vec3(-1.7f, 3.0f, -7.5f),
                                        glm::vec3(1.3f, -2.0f, -2.5f),
                                        glm::vec3(1.5f, 2.0f, -2.5f),
                                        glm::vec3(1.5f, 0.2f, -1.5f),
                                        glm::vec3(-1.3f, 1.0f, -1.5f)};*/

    unsigned int VBO, VAO;

    std::unique_ptr<core::Shader> shaderProgram;
    std::unique_ptr<core::Texture> texture1, texture2;

    float fov{45};
    float step{0.5};
    float m_mousePosXNorm, m_mousePosYNorm;
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
