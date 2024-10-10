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

    ImGui::InputText("Model path", m_guiData.m_modelPath, IM_ARRAYSIZE(m_guiData.m_modelPath));

    if(ImGui::Button("Load model"))
    {
        if(m_LoadModelCallBack)
        {
            m_LoadModelCallBack();
        }
    }

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

    // TODO check if it has actually changed?
    if(m_dataChangedCallBack)
    {
        m_dataChangedCallBack(m_guiData);
    }
}

void ImGuiLayer::onEvent(core::Event& e) {}

void ImGuiLayer::setCloseCallBack(std::function<void()> closeCallBack) { m_closeCallBack = closeCallBack; }

void ImGuiLayer::setDataChangedCallBack(std::function<void(const GuiData&)> dataChangedCallBack)
{
    m_dataChangedCallBack = dataChangedCallBack;
}

void ImGuiLayer::setLoadModelCallBack(std::function<void()> loadModelCallBack)
{
    m_LoadModelCallBack = loadModelCallBack;
}

void ImGuiLayer::setVisible(bool visible) { m_visible = visible; }

void ImGuiLayer::processInputs() {}
