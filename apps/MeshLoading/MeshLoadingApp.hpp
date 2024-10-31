#pragma once

#include "MeshLoadingLayer.hpp"
#include "OverlayInfoLayer.hpp"
#include "imgui.h"

#include <Application.hpp>
#include <Events/KeyEvent.hpp>
#include <Logger.hpp>

class MeshLoadingApp : public core::Application
{
  public:
    MeshLoadingApp()
      : core::Application("Model displayer")
      , m_meshLoadingLayer(std::make_shared<MeshLoadingLayer>(getWindow().getWidth(), getWindow().getHeight()))
      , m_overlayInfo(std::make_shared<OverlayInfoLayer>())
    {
        ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        getWindow().pushLayer(m_meshLoadingLayer);
        getWindow().pushOverlayLayer(m_overlayInfo);

        m_meshLoadingLayer->setDisplayOverlayChangedCallBack(
          [&](bool displayOverlay) { m_overlayInfo->setEnabled(displayOverlay); });
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(MeshLoadingApp::onWindowResized));
        dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(MeshLoadingApp::onKeyPressed));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        if(m_meshLoadingLayer)
        {
            m_meshLoadingLayer->setViewportSize(e.getWidth(), e.getHeight());
        }
        return false;
    }

    bool onKeyPressed(core::KeyPressedEvent& e)
    {
        if(e.getKeyCode() == GLFW_KEY_ESCAPE)
        {
            auto cursorMode = glfwGetInputMode(getWindow().getWindow(), GLFW_CURSOR);
            if(cursorMode == GLFW_CURSOR_DISABLED)
            {
                cursorMode = GLFW_CURSOR_NORMAL;
            }
            else
            {
                cursorMode = GLFW_CURSOR_DISABLED;
            }
            glfwSetInputMode(getWindow().getWindow(), GLFW_CURSOR, cursorMode);
            m_showUi = !m_showUi;
            m_meshLoadingLayer->setShowUi(m_showUi);
            m_cameraMovementEnabled = !m_cameraMovementEnabled;
            m_meshLoadingLayer->setCameraMovement(m_cameraMovementEnabled);
        }
        return false;
    }

  private:
    std::shared_ptr<MeshLoadingLayer> m_meshLoadingLayer;
    std::shared_ptr<OverlayInfoLayer> m_overlayInfo;

    bool m_showUi{false};
    bool m_cameraMovementEnabled{true};
};
