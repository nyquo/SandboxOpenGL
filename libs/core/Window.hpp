#pragma once

#include "Events/Event.hpp"
#include "Shader.hpp"

#include <gl.h>

#include <functional>
#include <string>

namespace core {

    class Window
    {
    public:
        Window(const std::string& name = "New Window", unsigned int width = 800, unsigned int height = 600);

        ~Window();

        void update();

        void setEventCallBack(std::function<void(Event&)>);

        // To remove later
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
    };

}