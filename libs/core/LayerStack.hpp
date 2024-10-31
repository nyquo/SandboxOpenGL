#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "Layer.hpp"

#include <memory>
#include <vector>

namespace core {

class Window;

class CORE_API LayerStack
{
  public:
    LayerStack(Window* window = nullptr);
    LayerStack(const LayerStack& other) = delete;
    LayerStack(LayerStack&& other) = delete;
    LayerStack operator=(const LayerStack& other) = delete;
    LayerStack operator=(LayerStack&& other) = delete;

    void pushLayer(std::shared_ptr<Layer> layer);
    void pushUiLayer(std::shared_ptr<Layer> layer);
    void removeLayer(std::shared_ptr<Layer> layer);
    void removeUiLayer(std::shared_ptr<Layer> layer);

    void onUpdate();
    void onEvent(Event& e);

  private:
    std::vector<std::shared_ptr<Layer>> m_layers;
    std::vector<std::shared_ptr<Layer>> m_uiLayers;

    Window* m_window;
};

}