#include "LayerStack.hpp"

#include <Window.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

namespace core {

LayerStack::LayerStack(Window* window)
  : m_window(window)
{
    if(window == nullptr)
    {
        throw std::invalid_argument("Layer stack created without a window");
    }
}

void LayerStack::pushLayer(const std::shared_ptr<Layer>& layer)
{
    if(std::find(m_layers.begin(), m_layers.end(), layer) == m_layers.end())
    {
        m_layers.push_back(layer);
    }
}

void LayerStack::pushOverlayLayer(const std::shared_ptr<Layer>& layer)
{
    if(std::find(m_uiLayers.begin(), m_uiLayers.end(), layer) == m_uiLayers.end())
    {
        m_uiLayers.push_back(layer);
    }
}

void LayerStack::removeLayer(const std::shared_ptr<Layer>& layer)
{
    auto itLayer = std::find(m_layers.begin(), m_layers.end(), layer);
    if(itLayer != m_layers.end())
    {
        m_layers.erase(itLayer);
    }
}

void LayerStack::removeOverlayLayer(const std::shared_ptr<Layer>& layer)
{
    auto itLayer = std::find(m_uiLayers.begin(), m_uiLayers.end(), layer);
    if(itLayer != m_uiLayers.end())
    {
        m_uiLayers.erase(itLayer);
    }
}

void LayerStack::onUpdate()
{
    for(auto& layer : m_layers)
    {
        if(layer->isEnabled())
        {
            layer->onUpdate();
        }
    }

    for(auto& layer : m_uiLayers)
    {
        if(layer->isEnabled())
        {
            layer->onUpdate();
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(m_window->getWidth(), m_window->getHeight());

    // // ---------- DOCKING VESRION 2
    // ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

    // // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // // because it would be confusing to have two docking targets within each others.
    // ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    // const ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(viewport->WorkPos);
    // ImGui::SetNextWindowSize(viewport->WorkSize);
    // ImGui::SetNextWindowViewport(viewport->ID);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // window_flags |=
    //   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
    //   ImGuiWindowFlags_NoMove;
    // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // // and handle the pass-thru hole, so we ask Begin() to not render a background.
    // window_flags |= ImGuiWindowFlags_NoBackground;

    // // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // // all active windows docked into it will lose their parent and become undocked.
    // // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    // ImGui::PopStyleVar(3);

    // // Submit the DockSpace
    // if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    // {
    //     ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    //     ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    // }

    // ImGui::End();
    // ---------- END ---- DOCKING VESRION 2

    // ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_Once); // Dock into the main DockSpace
    // ImGui::Begin("Main 3D Viewport",
    //              nullptr,
    //              /*ImGuiWindowFlags_NoTitleBar |*/ ImGuiWindowFlags_NoCollapse | /*ImGuiWindowFlags_NoResize |*/
    //                /*ImGuiWindowFlags_NoDocking |*/ /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoScrollbar |
    //                ImGuiWindowFlags_NoScrollWithMouse);

    // ImGui::End();

    // ---------- DOCKING VERSION 1
    // ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    // ---------- END ---- DOCKING VERSION 1

    for(auto& layer : m_layers)
    {
        if(layer->isEnabled())
        {
            layer->onImGuiRender();
        }
    }

    for(auto& layer : m_uiLayers)
    {
        if(layer->isEnabled())
        {
            layer->onImGuiRender();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void LayerStack::onEvent(Event& e)
{
    for(auto layer = m_uiLayers.rbegin(); layer != m_uiLayers.rend(); ++layer)
    {
        (*layer)->onEvent(e);
    }
    for(auto layer = m_layers.rbegin(); layer != m_layers.rend(); ++layer)
    {
        (*layer)->onEvent(e);
    }
}

}
