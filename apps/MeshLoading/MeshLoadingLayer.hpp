#pragma once

#include <BasicRenderer.hpp>
#include <Events/MouseEvent.hpp>
#include <Layer.hpp>
#include <Model.hpp>
#include <PerspectiveCamera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

struct ModelData
{
    std::string m_name;
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    bool m_outline{true};
};

struct GuiData
{
    char m_modelPath[128] = "/home/nicolas/Downloads/backpack/backpack.obj";

    renderer::DirectionalLight m_directionalLight{glm::vec3(-0.2f, -1.0f, -0.3f),
                                                  glm::vec3(0.2f, 0.2f, 0.2f),
                                                  glm::vec3(0.5f, 0.5f, 0.5f),
                                                  glm::vec3(1.0f, 1.0f, 1.0f)};

    std::vector<renderer::PointLight> m_pointLights{};
    std::vector<ModelData> m_models;

    bool m_displayOverlayInfo{true};
    bool m_oldDisplayOverlayInfo{false};

    bool m_enableMSAA{true};
    bool m_oldEnablerMSAA{false};
};

class MeshLoadingLayer : public core::Layer
{
  public:
    MeshLoadingLayer(float viewportWidth = 0, float viewportHeight = 0);
    ~MeshLoadingLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;
    void onImGuiRender() override;

    void updateData();
    void loadModel(fs::path path);

    void setViewportSize(float viewportWidth, float viewportHeight);
    void setCameraMovement(bool cameraMovementEnabled);
    void setShowUi(bool showUi);
    void setDisplayOverlayChangedCallBack(std::function<void(bool)> callBack);
    void setExitCallBack(std::function<void()> callBack);

  private:
    void processInputs();

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e);
    bool onMouseMoved(core::MouseMovedEvent& e);

    std::function<void(bool)> m_onDisplayOverlayChanged;
    std::function<void()> m_onExit;

  private:
    renderer::Scene m_scene;
    renderer::BasicRenderer m_renderer;
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;

    bool m_showUi{false};

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
