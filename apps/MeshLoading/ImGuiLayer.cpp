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

    ImGui::Text("Directional light");
    ImGui::DragFloat3("Direction", (float*)&m_guiData.m_directionalLight.m_direction);
    ImGui::ColorEdit3("D Ambient light color", (float*)&m_guiData.m_directionalLight.m_ambientColor);
    ImGui::ColorEdit3("D Diffuse light color", (float*)&m_guiData.m_directionalLight.m_diffuseColor);
    ImGui::ColorEdit3("D Specular light color", (float*)&m_guiData.m_directionalLight.m_specularColor);

    if(ImGui::Button("Add Point light"))
    {
        m_guiData.m_pointLights.emplace_back(renderer::PointLight(glm::vec3(0.0f, 1.0f, 0.0f),
                                                                  glm::vec3(0.2f, 0.2f, 0.2f),
                                                                  glm::vec3(0.5f, 0.5f, 0.5f),
                                                                  glm::vec3(1.0f, 1.0f, 1.0)));
    }

    for(int i = 0; i < m_guiData.m_pointLights.size(); ++i)
    {
        std::string name = "Point Light " + std::to_string(i);
        if(ImGui::TreeNode(name.c_str()))
        {
            ImGui::DragFloat3("position", (float*)&m_guiData.m_pointLights[i], 0.01f);
            ImGui::ColorEdit3("D Ambient light color", (float*)&m_guiData.m_pointLights[i].m_ambientColor);
            ImGui::ColorEdit3("D Diffuse light color", (float*)&m_guiData.m_pointLights[i].m_diffuseColor);
            ImGui::ColorEdit3("D Specular light color", (float*)&m_guiData.m_pointLights[i].m_specularColor);
            if(ImGui::Button("remove"))
            {
                m_guiData.m_pointLights.erase(std::next(m_guiData.m_pointLights.begin(), i));
            }
            ImGui::TreePop();
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

const GuiData& ImGuiLayer::getGuiData() const { return m_guiData; }

void ImGuiLayer::processInputs() {}
