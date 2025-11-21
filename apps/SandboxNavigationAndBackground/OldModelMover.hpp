#pragma once

#include <CameraMover.hpp>
#include <core/Events/MouseEvent.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>

class OldModelMover
{
  public:
    OldModelMover() = default;
    OldModelMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera,
                  const std::shared_ptr<glm::mat4>& modelMatrix)
      : m_camera(camera)
      , m_modelMatrix(modelMatrix){};
    OldModelMover(const OldModelMover& other) = delete;
    OldModelMover(OldModelMover&& other) = delete;
    OldModelMover operator=(const OldModelMover& other) = delete;
    OldModelMover operator=(OldModelMover&& other) = delete;
    virtual ~OldModelMover() = default;

    void onEvent(core::Event& event);
    void update();
    void init();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& event);
    bool onMouseMoved(core::MouseMovedEvent& event);
    bool onMouseButtonPressed(core::MouseButtonPressedEvent& event);
    bool onMouseButtonReleased(core::MouseButtonReleasedEvent& event);

  private:
    glm::vec2 m_currentMousePos{0.0F, 0.0F};
    glm::vec2 m_lastMousePos{0.0F, 0.0F};
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;
    std::shared_ptr<glm::mat4> m_modelMatrix;
    glm::vec3 m_target{0.0F, 0.0F, 0.0F};
    float m_distance{10.0f};
    bool m_enabled{true};
};