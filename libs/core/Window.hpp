#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Layer.hpp"
#include "gl.h"

#include <glm/glm.hpp>

#include <functional>
#include <memory>
#include <string>

namespace core {

class CORE_API Window
{
  public:
    Window(const std::string& name = "New Window", unsigned int width = 800, unsigned int height = 600);
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window operator=(const Window& other) = delete;
    Window operator=(Window&& other) = delete;
    ~Window();

    void onUpdate();
    void onEvent(Event& e);

    void setEventCallBack(std::function<void(Event&)>);

    GLFWwindow* getWindow() { return m_window; }

    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;

    [[nodiscard]] bool isKeyPressed(int key) const;
    [[nodiscard]] bool isMouseButtonPressed(int button) const;
    [[nodiscard]] glm::vec2 getMousePosition() const;

  private:
    static bool s_glfwInitialized;
    static bool s_gladInitialized;

    GLFWwindow* m_window;

    std::function<void(Event&)> m_eventCallBack;

    // Window settings
    std::string m_name;
    unsigned int m_width;
    unsigned int m_height;

    // TEMP
  private:
    // add layer stack?
    std::shared_ptr<Layer> m_mainLayer;
    std::shared_ptr<Layer> m_uiLayer;

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        m_width = e.getWidth();
        m_height = e.getHeight();
        return false;
    }

    // temp
  public:
    void setMainLayer(std::shared_ptr<Layer> layer)
    {
        m_mainLayer = std::move(layer);
        m_mainLayer->setWindow(this);
    }

    void setUiLayer(std::shared_ptr<Layer> layer)
    {
        m_uiLayer = std::move(layer);
        m_uiLayer->setWindow(this);
    }

    std::shared_ptr<Layer> getMainLayer() { return m_mainLayer; }
    std::shared_ptr<Layer> getUiLayer() { return m_uiLayer; }
};

}
