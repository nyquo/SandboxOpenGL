#pragma once

#include <CameraMover.hpp>
#include <core/Events/MouseEvent.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>

class TrackballCameraMover : public CameraMover
{
  public:
    TrackballCameraMover() = default;
    TrackballCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera);
    TrackballCameraMover(const TrackballCameraMover& other) = delete;
    TrackballCameraMover(TrackballCameraMover&& other) = delete;
    TrackballCameraMover operator=(const TrackballCameraMover& other) = delete;
    TrackballCameraMover operator=(TrackballCameraMover&& other) = delete;
    virtual ~TrackballCameraMover() = default;

    void onEvent(core::Event& event) override;
    void update() override;
    void init() override;

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& event);
    bool onMouseMoved(core::MouseMovedEvent& event);
    bool onMouseButtonPressed(core::MouseButtonPressedEvent& event);
    bool onMouseButtonReleased(core::MouseButtonReleasedEvent& event);

  private:
    glm::vec3 m_target{0.0F, 0.0F, 0.0F};
    float distance = 10.0f;

    float m_yaw = 0.0f;   // around y axis (world up)
    float m_pitch = 0.0f; // around x axis (camera right)
    float m_minPitch = glm::radians(-89.0f);
    float m_maxPitch = glm::radians(89.0f);

    float m_rotationSpeedX{0.3f};
    float m_rotationSpeedY{0.3f};

    glm::vec2 m_currentMousePos{0.0F, 0.0F};
    glm::vec2 m_lastMousePos{0.0F, 0.0F};
};