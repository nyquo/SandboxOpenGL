#pragma once

#include "PerspectiveCamera.hpp"
#include "RendererExport.hpp"

#include <core/Events/Event.hpp>
#include <core/Events/MouseEvent.hpp>
#include <core/Input.hpp>
#include <glm/glm.hpp>

#include <memory>

namespace renderer {

class RENDERER_API FpsCameraMover
{
  public:
    FpsCameraMover() = default;
    FpsCameraMover(const std::shared_ptr<PerspectiveCamera>& camera);
    FpsCameraMover(const FpsCameraMover& other) = delete;
    FpsCameraMover(FpsCameraMover&& other) = delete;
    FpsCameraMover& operator=(const FpsCameraMover& other) = delete;
    FpsCameraMover& operator=(FpsCameraMover&& other) = delete;
    ~FpsCameraMover() = default;

    void update();

    void setCamera(const std::shared_ptr<PerspectiveCamera>& camera);

    void onEvent(core::Event& e);

    void enable();
    void disable();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e);
    bool onMouseMoved(core::MouseMovedEvent& e);

  private:
    // TODO change that by a general Camera or Entity
    std::shared_ptr<PerspectiveCamera> m_camera;
    float m_zoomOffset{2.0};
    float m_mouseSensitivity{0.1};
    bool m_firstMouse{true};
    float m_lastMouseX{0};
    float m_lastMouseY{0};
    float m_movementSpeed{10.0};
    double m_deltaTime{0.0};
    double m_lastFrame{0.0};

    bool m_enabled{false};
};

}
