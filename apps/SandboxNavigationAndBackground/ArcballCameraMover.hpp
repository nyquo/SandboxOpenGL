#pragma once

#include <CameraMover.hpp>
#include <core/Events/MouseEvent.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>

class ArcballCameraMover : public CameraMover
{
  public:
    ArcballCameraMover() = default;
    ArcballCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera);
    ArcballCameraMover(const ArcballCameraMover& other) = delete;
    ArcballCameraMover(ArcballCameraMover&& other) = delete;
    ArcballCameraMover operator=(const ArcballCameraMover& other) = delete;
    ArcballCameraMover operator=(ArcballCameraMover&& other) = delete;
    virtual ~ArcballCameraMover() = default;

    void onEvent(core::Event& event) override;
    void update() override;
    void init() override;

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& event);
    bool onMouseMoved(core::MouseMovedEvent& event);
    bool onMouseButtonPressed(core::MouseButtonPressedEvent& event);
    bool onMouseButtonReleased(core::MouseButtonReleasedEvent& event);

  private:
    float m_zoomOffset{2.0};
    // double m_lastMouseX{0};
    // double m_lastMouseY{0};
    // bool m_firstMouse{true};
    float m_mouseSensitivityX{0.05};
    float m_mouseSensitivityY{0.05};

    // float m_centerPointDistance{10.0F};
    // double m_minHeight{-15.0};
    // double m_maxHeight{15.0};

    glm::vec3 m_originalPosition{0.0F, 0.0F, -10.0F};
    glm::vec2 m_firstMousePos{0.0F, 0.0F};
    glm::vec2 m_lastMousePos{0.0F, 0.0F};
};