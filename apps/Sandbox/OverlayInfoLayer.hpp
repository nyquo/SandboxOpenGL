#pragma once

#include "core/Layer.hpp"

class OverlayInfoLayer : public core::Layer
{
  public:
    void onImGuiRender() override;
};
