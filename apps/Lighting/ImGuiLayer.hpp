#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiLayer : public core::Layer
{
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;

    void setCloseCallBack(std::function<void()> closeCallBack);
    void setVisible(bool visible);

  private:
    void processInputs();

  private:
    std::function<void()> m_closeCallBack;

  private:
    bool m_visible{false};
};
