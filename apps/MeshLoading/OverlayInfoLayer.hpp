#pragma once

#include "Layer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class OverlayInfoLayer : public core::Layer
{
  public:
    void onUpdate();
    void onEvent(core::Event& e) override;

    void setVisible(bool visible);

  private:
    bool m_visible{false};
};
