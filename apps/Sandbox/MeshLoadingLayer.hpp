#pragma once

#include <BasicRenderer.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <FpsCameraMover.hpp>
#include <Layer.hpp>
#include <Model.hpp>
#include <PerspectiveCamera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

namespace fs = std::filesystem;

struct ModelData
{
    std::string m_name;
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    bool m_outline{true};
};

struct GuiData
{
    char m_modelPath[128] = "";
    char m_spritePath[128] = "";

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

    bool m_wireFrame{false};
    bool m_oldWireFrame{true};
};

class MeshLoadingLayer : public core::Layer
{
  public:
    MeshLoadingLayer(float layerWidth = 0, float layerHeight = 0);
    ~MeshLoadingLayer();

    void onUpdate() override;
    void onEvent(core::Event& e) override;
    void onImGuiRender() override;

    void updateData();
    void loadModel(fs::path path);
    void loadSprite(fs::path path);

    void setLayerSize(float width, float height);
    void setDisplayOverlayChangedCallBack(std::function<void(bool)> callBack);
    void setExitCallBack(std::function<void()> callBack);

  private:
    void processInputs();

  private:
    bool onKeyPressed(core::KeyPressedEvent& e);
    bool onMouseButtonPressed(core::MouseButtonPressedEvent& e);

    std::function<void(bool)> m_onDisplayOverlayChanged;
    std::function<void()> m_onExit;

  private:
    renderer::Scene m_scene;
    renderer::BasicRenderer m_renderer;
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;
    renderer::FpsCameraMover m_fpsCameraMover{m_camera};

    bool m_showUi{true};

    double m_glfwTimeCount{0};
    float m_lightCubePositionOffset{0};
    GuiData m_guiData;

    float m_layerWidth;
    float m_layerHeight;

    bool m_cameraMovementEnabled{true};

    // viewport
    ImVec2 m_vMin{0.0f, 0.0f};
    ImVec2 m_vMax{800.0f, 600.0f};
};
