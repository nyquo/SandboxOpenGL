#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

struct GuiData
{
    int m_shininess{32};

    std::vector<glm::vec3> m_lightCubePos{glm::vec3(0.7f, 0.2f, 2.0f),
                                          glm::vec3(2.3f, -3.3f, -4.0f),
                                          glm::vec3(-4.0f, 2.0f, -12.0f),
                                          glm::vec3(0.0f, 0.0f, -3.0f)};

    ImVec4 m_ambientPointLight{0.2f, 0.2f, 0.2f, 1.0f};
    ImVec4 m_diffusePointLight{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 m_specularPointLight{1.0f, 1.0f, 1.0, 1.0f};

    glm::vec3 m_dirLightDirection{-0.2f, -1.0f, -0.3f};
    ImVec4 m_ambientDirLight{0.2f, 0.2f, 0.2f, 1.0f};
    ImVec4 m_diffuseDirLight{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 m_specularDirLight{1.0f, 1.0f, 1.0, 1.0f};

    ImVec4 m_ambientFlashLight{0.2f, 0.2f, 0.2f, 1.0f};
    ImVec4 m_diffuseFlashLight{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 m_specularFlashLight{1.0f, 1.0f, 1.0, 1.0f};

    float m_cutOff{12.5f};
    float m_outerCutOff{17.5};

    char m_modelPath[128];
};

class ImGuiLayer : public core::Layer
{
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;

    void setCloseCallBack(std::function<void()> closeCallBack);
    void setDataChangedCallBack(std::function<void(const GuiData&)> dataChangedCallBack);
    void setLoadModelCallBack(std::function<void()> loadModelCallBack);
    void setVisible(bool visible);

  private:
    void processInputs();

  private:
    std::function<void()> m_closeCallBack;
    std::function<void(const GuiData&)> m_dataChangedCallBack;
    std::function<void()> m_LoadModelCallBack;

  private:
    bool m_visible{false};

    GuiData m_guiData;
};
