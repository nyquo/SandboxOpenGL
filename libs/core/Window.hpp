#pragma once

#include "Events/Event.hpp"
#include "Layer.hpp"
#include "Shader.hpp"

#include <gl.h>

#include <functional>
#include <string>

namespace core {

class Window
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
    std::unique_ptr<Layer> m_mainLayer;

  public:
    void setLayer(std::unique_ptr<Layer> layer) { m_mainLayer = std::move(layer); }
};

}
