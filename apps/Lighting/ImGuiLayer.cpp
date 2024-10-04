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
    ImGui::SliderFloat("Light cube speed", &m_guiData.m_lightCubeSpeed, 0.0f, 5.0f);

    ImGui::ColorEdit3("Ambient cube mat", (float*)&m_guiData.m_ambientColor);
    ImGui::ColorEdit3("Diffuse cube mat", (float*)&m_guiData.m_diffuseColor);
    ImGui::ColorEdit3("Specular cube mat", (float*)&m_guiData.m_specularColor);
    ImGui::SliderInt("Shininess", &m_guiData.m_shininess, 2, 512);

    ImGui::ColorEdit3("Ambient light color", (float*)&m_guiData.m_ambientLight);
    ImGui::ColorEdit3("Diffuse light color", (float*)&m_guiData.m_diffuseLight);
    ImGui::ColorEdit3("Specular light color", (float*)&m_guiData.m_specularLight);

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
