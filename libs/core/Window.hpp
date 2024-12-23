#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
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

    void pushLayer(std::shared_ptr<Layer> layer);
    void pushOverlayLayer(std::shared_ptr<Layer> layer);
    void removeLayer(std::shared_ptr<Layer> layer);
    void removeOverlayLayer(std::shared_ptr<Layer> layer);

  private:
    static bool s_glfwInitialized;
    static bool s_gladInitialized;

    GLFWwindow* m_window;
    LayerStack m_layerStack{this};

    std::function<void(Event&)> m_eventCallBack;

    // Window settings
    std::string m_name;
    unsigned int m_width;
    unsigned int m_height;

  private:
    bool onWindowResized(core::WindowResizeEvent& e)
    {
        m_width = e.getWidth();
        m_height = e.getHeight();
        return false;
    }
};

}
