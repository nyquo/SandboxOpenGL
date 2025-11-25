#pragma once

#include <core/Events/Event.hpp>
#include <memory>
#include <renderer/PerspectiveCamera.hpp>
class CameraMover
{
  public:
    CameraMover() = default;
    CameraMover(const std::shared_ptr<renderer::PerspectiveCamera>& camera)
      : m_camera(camera)
    {}
    CameraMover(const CameraMover& other) = delete;
    CameraMover(CameraMover&& other) = delete;
    CameraMover operator=(const CameraMover& other) = delete;
    CameraMover operator=(CameraMover&& other) = delete;
    virtual ~CameraMover() = default;

    virtual void onEvent(core::Event& event) = 0;
    virtual void update() = 0;
    virtual void init() = 0;

    void setCamera(const std::shared_ptr<renderer::PerspectiveCamera>& camera) { m_camera = camera; }
    void enable() { m_enabled = true; };
    void disable() { m_enabled = false; };
    void setMouseSensitivity(float sensitivityX, float sensitivityY)
    {
        m_mouseSensitivityX = sensitivityX;
        m_mouseSensitivityY = sensitivityY;
    }

  protected:
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;
    float m_mouseSensitivityX{0.3};
    float m_mouseSensitivityY{0.3};

    bool m_enabled{true};
};