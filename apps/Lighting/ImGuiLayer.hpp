#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct GuiData
{
    float m_lightCubeSpeed{1};

    ImVec4 m_ambientColor{1.0f, 0.5f, 0.31f, 1.0f};
    ImVec4 m_diffuseColor{1.0f, 0.5f, 0.31f, 1.0f};
    ImVec4 m_specularColor{0.5f, 0.5f, 0.5f, 1.0f};
    int m_shininess{32};

    ImVec4 m_ambientLight{0.2f, 0.2f, 0.2f, 1.0f};
    ImVec4 m_diffuseLight{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 m_specularLight{1.0f, 1.0f, 1.0, 1.0f};
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
