#include "LayerStack.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace core {

LayerStack::LayerStack(Window* window)
  : m_window(window)
{}

void LayerStack::pushLayer(std::shared_ptr<Layer> layer)
{
    if(std::find(m_layers.begin(), m_layers.end(), layer) == m_layers.end())
    {
        layer->setWindow(m_window);
        m_layers.push_back(layer);
    }
}

void LayerStack::pushUiLayer(std::shared_ptr<Layer> layer)
{
    if(std::find(m_uiLayers.begin(), m_uiLayers.end(), layer) == m_uiLayers.end())
    {
        layer->setWindow(m_window);
        m_uiLayers.push_back(layer);
    }
}

void LayerStack::removeLayer(std::shared_ptr<Layer> layer)
{
    auto itLayer = std::find(m_layers.begin(), m_layers.end(), layer);
    if(itLayer != m_layers.end())
    {
        m_layers.erase(itLayer);
    }
}

void LayerStack::removeUiLayer(std::shared_ptr<Layer> layer)
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
        layer->onUpdate();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for(auto& layer : m_uiLayers)
    {
        layer->onUpdate();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
