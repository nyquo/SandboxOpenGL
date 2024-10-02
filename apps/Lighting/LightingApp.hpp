#pragma once

#include "ViewportLayer.hpp"

#include <Application.hpp>

class LightingApp : public core::Application
{
  public:
    LightingApp()
    {
        // glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        getWindow().setLayer(std::make_shared<ViewportLayer>(getWindow().getWidth(), getWindow().getHeight()));
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(LightingApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        auto viewportLayer = std::dynamic_pointer_cast<ViewportLayer>(getWindow().getLayer());
        viewportLayer->setViewportSize(e.getWidth(), e.getHeight());
        return false;
    }
};
