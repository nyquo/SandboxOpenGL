#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer() {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::onUpdate()
{
    if(!m_visible)
    {
        return;
    }
    ImGui::NewFrame();

    ImGui::Begin("Options Window");

    ImGui::Text("Light cube positions");
    ImGui::DragFloat3("Cube 1", (float*)&m_guiData.m_lightCubePos[0], 0.01f);
    ImGui::DragFloat3("Cube 2", (float*)&m_guiData.m_lightCubePos[1], 0.01f);
    ImGui::DragFloat3("Cube 3", (float*)&m_guiData.m_lightCubePos[2], 0.01f);
    ImGui::DragFloat3("Cube 4", (float*)&m_guiData.m_lightCubePos[3], 0.01f);

    ImGui::Text("Directional lights");
    ImGui::DragFloat3("Direction", (float*)&m_guiData.m_dirLightDirection);
    ImGui::ColorEdit3("D Ambient light color", (float*)&m_guiData.m_ambientDirLight);
    ImGui::ColorEdit3("D Diffuse light color", (float*)&m_guiData.m_diffuseDirLight);
    ImGui::ColorEdit3("D Specular light color", (float*)&m_guiData.m_specularDirLight);

    ImGui::Text("Point lights");
    ImGui::ColorEdit3("P Ambient light color", (float*)&m_guiData.m_ambientPointLight);
    ImGui::ColorEdit3("P Diffuse light color", (float*)&m_guiData.m_diffusePointLight);
    ImGui::ColorEdit3("P Specular light color", (float*)&m_guiData.m_specularPointLight);

    ImGui::Text("Flash light");
    ImGui::SliderInt("Shininess", &m_guiData.m_shininess, 2, 512);
    ImGui::SliderFloat("CutOff", &m_guiData.m_cutOff, 0, 30);
    ImGui::SliderFloat("Outer CutOff", &m_guiData.m_outerCutOff, m_guiData.m_cutOff, m_guiData.m_cutOff * 2);
    ImGui::ColorEdit3("F Ambient light color", (float*)&m_guiData.m_ambientFlashLight);
    ImGui::ColorEdit3("F Diffuse light color", (float*)&m_guiData.m_diffuseFlashLight);
    ImGui::ColorEdit3("F Specular light color", (float*)&m_guiData.m_specularFlashLight);

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
}

void ImGuiLayer::onEvent(core::Event& e) {}

void ImGuiLayer::setCloseCallBack(std::function<void()> closeCallBack) { m_closeCallBack = closeCallBack; }

void ImGuiLayer::setDataChangedCallBack(std::function<void(const GuiData&)> dataChangedCallBack)
{
    m_dataChangedCallBack = dataChangedCallBack;
}

void ImGuiLayer::setVisible(bool visible) { m_visible = visible; }

void ImGuiLayer::processInputs() {}
