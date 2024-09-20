#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "Layer.hpp"
#include "gl.h"

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

    void update();

    void setEventCallBack(std::function<void(Event&)>);

    GLFWwindow* getWindow() { return m_window; }

    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;

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

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        m_width = e.getWidth();
        m_height = e.getHeight();
        return false;
    }

  public:
    // Move this elsewhere ? Put layers in app?
    void onEvent(Event& e);
    void setLayer(std::shared_ptr<Layer> layer) { m_mainLayer = std::move(layer); }
    std::shared_ptr<Layer> getLayer() { return m_mainLayer; }
};

}
