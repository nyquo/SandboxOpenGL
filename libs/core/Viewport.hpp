#pragma once

#include <core/Events/Event.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <string>

namespace core {

class Viewport
{
  public:
    Viewport(float m_layerWidth,
             float m_layerHeight,
             std::string viewportName = "Viewport",
             float x = 0,
             float y = 0,
             float width = 800,
             float height = 600,
             glm::vec3 backgroundColor = glm::vec3(0.3f, 0.3f, 0.3f));
    ~Viewport();

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    virtual void onEvent(core::Event& event) = 0;
    virtual void onUpdate() = 0;

    void displayViewportWindow();
    void setLayerSize(float width, float height);

    void setBackgroundColor(const glm::vec3& color) { m_backgroundColor = color; }
    bool isActive() const { return m_isActive; }

  protected:
    void begin();
    void end();

  private:
    ImVec2 m_vMin{0.0f, 0.0f};
    ImVec2 m_vMax{800.0f, 600.0f};

    float m_x;
    float m_y;
    float m_width;
    float m_height;
    float m_layerWidth;
    float m_layerHeight;
    bool m_isActive{true};

    glm::vec3 m_backgroundColor;

    std::string m_viewportName;
};

}