#pragma once

#include "ImGuiLayer.hpp"
#include "ViewportLayer.hpp"

#include <Application.hpp>

class LightingApp : public core::Application
{
  public:
    LightingApp()
    {
#ifdef NDEBUG
        glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
        getWindow().setMainLayer(std::make_shared<ViewportLayer>(getWindow().getWidth(), getWindow().getHeight()));

        auto imGuiLayer = std::make_shared<ImGuiLayer>();
        imGuiLayer->setCloseCallBack([this]() { glfwSetWindowShouldClose(getWindow().getWindow(), GLFW_TRUE); });
        getWindow().setUiLayer(imGuiLayer);
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(LightingApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        auto viewportLayer = std::dynamic_pointer_cast<ViewportLayer>(getWindow().getMainLayer());
        viewportLayer->setViewportSize(e.getWidth(), e.getHeight());
        return false;
    }
};
