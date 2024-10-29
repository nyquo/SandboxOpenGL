#pragma once
#include "ImGuiLayer.hpp"

#include <BasicRenderer.hpp>
#include <Events/MouseEvent.hpp>
#include <Layer.hpp>
#include <Model.hpp>
#include <PerspectiveCamera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
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
    void setGuiData(const GuiData& guiData);

    void loadModel(std::shared_ptr<renderer::Model> model);

  private:
    void processInputs();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e);
    bool onMouseMoved(core::MouseMovedEvent& e);

  private:
    renderer::Scene m_scene;
    renderer::BasicRenderer m_renderer;
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;

    // TEMP
    // std::vector<glm::mat4> m_cubeModelMatrix;

    // std::unique_ptr<core::Texture> m_cubeTexture;
    // std::unique_ptr<core::Texture> m_cubeSpecularTexture;

    // std::unique_ptr<core::Shader> m_lightCubeShader;

    // BasicCube m_lightCube{};

    double m_glfwTimeCount{0};
    float m_lightCubePositionOffset{0};
    GuiData m_guiData;

    float m_viewportWidth;
    float m_viewportHeight;

    bool m_cameraMovementEnabled{true};

    // Camera var
    float m_zoomOffset{2.0};
    float m_mouseSensitivity{0.1F};
    bool m_firstMouse{true};
    float m_lastMouseX;
    float m_lastMouseY;
    float m_movementSpeed = 10.0F;
    float m_deltaTime{0.0};
    float m_lastFrame{0.0};
};
