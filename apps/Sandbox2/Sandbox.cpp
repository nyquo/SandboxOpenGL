#include "Sandbox.hpp"

SandboxApp::SandboxApp()
  : core::Application("Sandbox")
  , m_editorLayer(std::make_shared<EditorLayer>(getWindow().getWidth(), getWindow().getHeight()))
{
    getWindow().pushLayer(m_editorLayer);
}

void SandboxApp::onEvent(core::Event& e)
{
    core::Application::onEvent(e);

    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(SandboxApp::onWindowResized));
}

bool SandboxApp::onWindowResized(core::WindowResizeEvent& e)
{
    if(m_editorLayer)
    {
        m_editorLayer->setLayerSize(e.getWidth(), e.getHeight());
    }
    return false;
}
