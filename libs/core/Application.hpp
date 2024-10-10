#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "Window.hpp"

#include <atomic>

namespace core {

class CORE_API Application
{
  public:
    Application(std::string mainWindowName = "HelloWord");
    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
    Application operator=(const Application& other) = delete;
    Application operator=(Application&& other) = delete;
    ~Application();

  public:
    void run();

  protected:
    Window& getWindow() { return *m_mainWindow; }

  public:
    virtual void onEvent(Event& e);

  private: // Event callbacks
    bool onWindowClose(WindowCloseEvent& e);
    bool onWindowResized(WindowResizeEvent& e);
    bool onKeyPressed(core::KeyPressedEvent& e);

  private:
    std::unique_ptr<Window> m_mainWindow;
    std::atomic_bool m_continueRunning{true};
};

}
