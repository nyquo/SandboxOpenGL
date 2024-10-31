#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "gl.h"

#include <glm/glm.hpp>

namespace core {

class Window;

class CORE_API Layer
{
  public:
    Layer(Window* window = nullptr);
    Layer(const Layer& other) = delete;
    Layer(Layer&& other) = delete;
    Layer operator=(const Layer& other) = delete;
    Layer operator=(Layer&& other) = delete;

    virtual void onUpdate() = 0;
    virtual void onEvent(Event& event) = 0;

    [[nodiscard]] bool isKeyPressed(int key) const;
    [[nodiscard]] bool isMouseButtonPressed(int button) const;
    [[nodiscard]] glm::vec2 getMousePosition() const;

    void setWindow(Window* window);

  private:
    Window* m_window;
};

}
