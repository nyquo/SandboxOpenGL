#pragma once

#include <BasicCube.hpp>
#include <Events/MouseEvent.hpp>
#include <Layer.hpp>
#include <PerspectiveCamera.hpp>
#include <Shader.hpp>
#include <glm/glm.hpp>

class ViewportLayer : public core::Layer
{
  public:
    ViewportLayer(float viewportWidth = 0, float viewportHeight = 0);
    ~ViewportLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;

    void setViewportSize(float viewportWidth, float viewportHeight);

    void setCameraMovement(bool cameraMovementEnabled);

  private:
    void processInputs();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e);
    bool onMouseMoved(core::MouseMovedEvent& e);

  private:
    BasicCube m_cube{};
    BasicCube m_lightCube{};

    unsigned VBO, VAO;
    unsigned m_lightVAO;
    std::unique_ptr<core::Shader> m_cubeShader, m_lightCubeShader;

    float m_viewportWidth;
    float m_viewportHeight;

    bool m_cameraMovementEnabled{true};

    // Camera var
    core::PerspectiveCamera m_camera;
    float m_zoomOffset{2.0};
    float m_mouseSensitivity{0.1F};
    bool m_firstMouse{true};
    float m_lastMouseX;
    float m_lastMouseY;
    float m_movementSpeed = 10.0F;
    float m_deltaTime{0.0};
    float m_lastFrame{0.0};
};
