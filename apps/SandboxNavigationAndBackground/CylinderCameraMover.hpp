#pragma once

#include <CameraMover.hpp>
#include <core/Events/MouseEvent.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>

class CylinderCameraMover : public CameraMover
{
  public:
    CylinderCameraMover() = default;
    CylinderCameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera);
    CylinderCameraMover(const CylinderCameraMover& other) = delete;
    CylinderCameraMover(CylinderCameraMover&& other) = delete;
    CylinderCameraMover operator=(const CylinderCameraMover& other) = delete;
    CylinderCameraMover operator=(CylinderCameraMover&& other) = delete;
    virtual ~CylinderCameraMover() = default;

    void onEvent(core::Event& event) override;
    void update() override;
    void init() override;

  private:
    void moveCamera(double horizontalAngleOffset, double verticalOffset);
    static glm::vec3 rotateAroundPoint(const glm::vec3& pointToRotate,
                                       const glm::vec3& pivot,
                                       float angleRadians,
                                       const glm::vec3& axis = glm::vec3(0.0F, 1.0F, 0.0F));

    bool onMouseScrolled(core::MouseScrolledEvent& event);
    bool onMouseMoved(core::MouseMovedEvent& event);

  private:
    float m_zoomOffset{2.0};
    double m_lastMouseX{0};
    double m_lastMouseY{0};
    bool m_firstMouse{true};

    float m_centerPointDistance{10.0F};
    double m_minHeight{-15.0};
    double m_maxHeight{15.0};
};