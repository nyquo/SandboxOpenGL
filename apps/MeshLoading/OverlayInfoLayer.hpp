#pragma once

#include "Layer.hpp"

class OverlayInfoLayer : public core::Layer
{
  public:
    void onImGuiRender() override;
};
