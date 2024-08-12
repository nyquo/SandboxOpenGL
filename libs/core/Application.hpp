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

    protected:
        Window& getWindow() { return *m_mainWindow; }

    private:
        void onEvent(Event& e);
        
    private: // Event callbacks
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResized(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> m_mainWindow;
        std::atomic_bool m_continueRunning{ true };
    };

}