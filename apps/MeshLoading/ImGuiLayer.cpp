#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer() { glDisable(GL_MULTISAMPLE); }

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::onUpdate()
{
    if(!m_visible)
    {
        return;
    }

    ImGui::Begin("Options Window");

    ImGui::InputText("Model path", m_guiData.m_modelPath, IM_ARRAYSIZE(m_guiData.m_modelPath));

    if(ImGui::Button("Load model"))
    {
        if(m_LoadModelCallBack)
        {
            m_LoadModelCallBack();
        }
    }

    ImGui::Checkbox("Enable Info Overlay", &m_guiData.m_enableOverlayInfo);
    ImGui::Checkbox("Enable MSAA", &m_guiData.m_enableMSAA);

    if(ImGui::Button("Exit app"))
    {
        if(m_closeCallBack)
        {
            m_closeCallBack();
        }
    }
    ImGui::End();

    // TODO check if it has actually changed?
    if(m_dataChangedCallBack)
    {
        m_dataChangedCallBack(m_guiData);
    }

    if(m_guiData.m_enableMSAA != m_guiData.m_oldEnablerMSAA)
    {
        m_guiData.m_oldEnablerMSAA = m_guiData.m_enableMSAA;
        if(m_guiData.m_enableMSAA)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
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
