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
    ImGui::DragFloat3("Light cube pos", (float*)&m_guiData.m_lightCubePosition, 0.01f);

    ImGui::SliderInt("Shininess", &m_guiData.m_shininess, 2, 512);

    ImGui::ColorEdit3("Ambient light color", (float*)&m_guiData.m_ambientLight);
    ImGui::ColorEdit3("Diffuse light color", (float*)&m_guiData.m_diffuseLight);
    ImGui::ColorEdit3("Specular light color", (float*)&m_guiData.m_specularLight);

    ImGui::SliderFloat("CutOff", &m_guiData.m_cutOff, 0, 30);
    ImGui::SliderFloat("Outer CutOff", &m_guiData.m_outerCutOff, m_guiData.m_cutOff, m_guiData.m_cutOff * 2);

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

void ImGuiLayer::setVisible(bool visible) { m_visible = visible; }

void ImGuiLayer::processInputs() {}
