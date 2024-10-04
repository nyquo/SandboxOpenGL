#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct GuiData
{
    glm::vec3 m_lightCubePosition{2.0F, 2.0f, 2.0f};

    ImVec4 m_specularColor{0.5f, 0.5f, 0.5f, 1.0f};
    int m_shininess{32};

    ImVec4 m_ambientLight{0.2f, 0.2f, 0.2f, 1.0f};
    ImVec4 m_diffuseLight{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 m_specularLight{1.0f, 1.0f, 1.0, 1.0f};

    float m_cutOff{12.5f};
    float m_outerCutOff{17.5};
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
    void setVisible(bool visible);

  private:
    void processInputs();

  private:
    std::function<void()> m_closeCallBack;
    std::function<void(const GuiData&)> m_dataChangedCallBack;

  private:
    bool m_visible{false};

    GuiData m_guiData;
};
