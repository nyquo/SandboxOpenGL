#pragma once

#include <core/Events/Event.hpp>
#include <glm/glm.hpp>

class ViewportScene
{
  public:
    ViewportScene() = default;
    virtual ~ViewportScene() = default;

    virtual void onEvent(core::Event& event);
    virtual void onUpdate();
    virtual void setViewport(float x, float y, float width, float height);

  private:
    void begin();
    void end();

  private:
    float m_x{0.0f};
    float m_y{0.0f};
    float m_width{800.0f};
    float m_height{600.0f};

    glm::vec3 m_backgroundColor{0.3f, 0.3f, 0.3f};
};