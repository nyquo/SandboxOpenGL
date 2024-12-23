#include "Application.hpp"

namespace core {

Application::Application(std::string mainWindowName)
  : m_mainWindow(std::make_unique<Window>(mainWindowName, 1800, 1000))
{
    m_mainWindow->setEventCallBack(BIND_EVENT_FN(Application::onEvent));
}

Application::~Application() { glfwTerminate(); }

void Application::run()
{
    while(m_continueRunning && !glfwWindowShouldClose(m_mainWindow->getWindow()))
    {
        m_mainWindow->onUpdate();
    }
}

void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
    dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));

    m_mainWindow->onEvent(e);
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
    m_continueRunning = false;
    return false;
}

bool Application::onKeyPressed(core::KeyPressedEvent& e) { return false; }

}
