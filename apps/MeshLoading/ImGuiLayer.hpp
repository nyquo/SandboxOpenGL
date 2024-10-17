#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

struct GuiData
{
    char m_modelPath[128] = "/home/nicolas/Downloads/backpack/backpack.obj";

    bool m_enableMSAA{false};
    bool m_oldEnablerMSAA{false};
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
