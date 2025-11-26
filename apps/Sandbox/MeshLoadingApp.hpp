#pragma once

#include "MeshLoadingLayer.hpp"
#include "OverlayInfoLayer.hpp"
#include "imgui.h"

#include <core/Application.hpp>
#include <core/Logger.hpp>

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

        getWindow().pushLayer(m_meshLoadingLayer);
        // getWindow().pushOverlayLayer(m_overlayInfo);

        m_meshLoadingLayer->setDisplayOverlayChangedCallBack(
          [&](bool displayOverlay) { m_overlayInfo->setEnabled(displayOverlay); });
        m_meshLoadingLayer->setExitCallBack([&]() { glfwSetWindowShouldClose(getWindow().getWindow(), GLFW_TRUE); });
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(MeshLoadingApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        if(m_meshLoadingLayer)
        {
            m_meshLoadingLayer->setLayerSize(e.getWidth(), e.getHeight());
        }
        return false;
    }

  private:
    std::shared_ptr<MeshLoadingLayer> m_meshLoadingLayer;
    std::shared_ptr<OverlayInfoLayer> m_overlayInfo;

    bool m_showUi{false};
    bool m_cameraMovementEnabled{true};
};
