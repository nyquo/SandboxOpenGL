#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer() {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::onUpdate()
{
    if(!m_visible)
    {
        return;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Options Window");
    if(ImGui::Button("Exit app"))
    {
        if(m_closeCallBack)
        {
            m_closeCallBack();
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(core::Event& e) {}

void ImGuiLayer::setCloseCallBack(std::function<void()> closeCallBack) { m_closeCallBack = closeCallBack; }

void ImGuiLayer::setVisible(bool visible) { m_visible = visible; }

void ImGuiLayer::processInputs() {}
