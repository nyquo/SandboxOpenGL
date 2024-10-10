#pragma once

#include "ImGuiLayer.hpp"
#include "ViewportLayer.hpp"

#include <Application.hpp>
#include <Events/KeyEvent.hpp>

class MeshLoadingApp : public core::Application
{
  public:
    MeshLoadingApp()
      : core::Application("Model displayer")
      , m_imGuiLayer(std::make_shared<ImGuiLayer>())
      , m_viewportLayer(std::make_shared<ViewportLayer>(getWindow().getWidth(), getWindow().getHeight()))
    {
        glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        getWindow().setMainLayer(m_viewportLayer);

        m_imGuiLayer->setCloseCallBack([this]() { glfwSetWindowShouldClose(getWindow().getWindow(), GLFW_TRUE); });
        m_imGuiLayer->setDataChangedCallBack([this](const GuiData& data) { m_viewportLayer->setGuiData(data); });
        getWindow().setUiLayer(m_imGuiLayer);
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(MeshLoadingApp::onWindowResized));
        dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(MeshLoadingApp::onKeyPressed));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        auto viewportLayer = std::dynamic_pointer_cast<ViewportLayer>(getWindow().getMainLayer());
        viewportLayer->setViewportSize(e.getWidth(), e.getHeight());
        return false;
    }

    bool onKeyPressed(core::KeyPressedEvent& e)
    {
        if(e.getKeyCode() == GLFW_KEY_ESCAPE)
        {
            auto cursorMode = glfwGetInputMode(getWindow().getWindow(), GLFW_CURSOR);
            if(cursorMode == GLFW_CURSOR_DISABLED)
            {
                cursorMode = GLFW_CURSOR_NORMAL;
            }
            else
            {
                cursorMode = GLFW_CURSOR_DISABLED;
            }
            glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, cursorMode);
            m_showImGuiLayer = !m_showImGuiLayer;
            m_imGuiLayer->setVisible(m_showImGuiLayer);
            m_cameraMovementEnabled = !m_cameraMovementEnabled;
            m_viewportLayer->setCameraMovement(m_cameraMovementEnabled);
        }
        return false;
    }

  private:
    std::shared_ptr<ImGuiLayer> m_imGuiLayer;
    std::shared_ptr<ViewportLayer> m_viewportLayer;

    bool m_showImGuiLayer{false};
    bool m_cameraMovementEnabled{true};
};
