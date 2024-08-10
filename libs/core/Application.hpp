#pragma once

#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "Window.hpp"
#include "gl.h"

#include <atomic>

namespace core {

#define BIND_EVENT_FN(e) std::bind(&e, this, std::placeholders::_1)

    class Application
    {
    public:
        Application();
        Application(const Application& other) = delete;
        Application(Application&& other) = delete;
        Application operator=(const Application& other) = delete;
        Application operator=(Application&& other) = delete;
        ~Application();

    public:
        void run();

    private:
        void onEvent(Event& e);

        std::atomic_bool m_continueRunning{ true };

        // event callbacks
    private:
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResized(WindowResizeEvent& e);

    private:
        Window m_mainWindow{ "Hello word", 800, 600 };
    };

}