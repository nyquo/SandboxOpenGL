#include "EditorLayer.hpp"

#include <algorithm>
#include <core/Input.hpp>
#include <core/Logger.hpp>
#include <core/gl.h>

EditorLayer::EditorLayer(float width, float height)
  : m_layerWidth(width)
  , m_layerHeight(height)
{}

EditorLayer::~EditorLayer() {}

void EditorLayer::onUpdate()
{
    glViewport(0, 0, m_layerWidth, m_layerHeight);
    glClearColor(m_windowBackgroundColor.r, m_windowBackgroundColor.g, m_windowBackgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_meshLoadingScene.setViewport(m_viewportData.x, m_viewportData.y, m_viewportData.width, m_viewportData.height);
    m_meshLoadingScene.update();
}

void EditorLayer::onImGuiRender()
{
    displayOptionWindow();
    displayViewportWindow();
}

void EditorLayer::onEvent(core::Event& e)
{
    m_meshLoadingScene.onEvent(e);
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::FileDropEvent>(BIND_EVENT_FN(EditorLayer::onFileDropped));
}

void EditorLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
}

void EditorLayer::displayOptionWindow()
{
    float fps = ImGui::GetIO().Framerate;
    ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Option Window");

    // FPS Display
    ImGui::Text("FPS: %.1f", fps);

    ImGui::NewLine();

    // Sensibility Settings
    ImGui::SliderFloat("Sensitiviy", &m_meshLoadingScene.getOptions().sensitivity, 0.0f, 1.0f);
    ImGui::Checkbox("Touchscreen mode", &m_meshLoadingScene.getOptions().displayGrid);

    // Mesh loading controls
    ImGui::InputText(
      "Mesh Path", m_meshLoadingScene.getOptions().meshPath, IM_ARRAYSIZE(m_meshLoadingScene.getOptions().meshPath));
    ImGui::SameLine();
    if(ImGui::Button("Load Mesh"))
    {
        m_meshLoadingScene.loadMeshFromFile(m_meshLoadingScene.getOptions().meshPath);
    }

    // Background modes
    std::vector<const char*> backgroundModes = {"Single Color", "Vignette", "Gradient"};
    static int currentBackgroundMode = 0;
    ImGui::Combo(
      "Background Mode", &currentBackgroundMode, backgroundModes.data(), static_cast<int>(backgroundModes.size()));
    float indent = 20.0f;
    ImGui::Indent(indent);
    switch(currentBackgroundMode)
    {
        case 0: {
            m_meshLoadingScene.getOptions().backgroundMode = MeshLoadingSceneOptions::BackgroundMode::SingleColor;
            optionSingleColorBackground();
            break;
        }
        case 1: {
            m_meshLoadingScene.getOptions().backgroundMode = MeshLoadingSceneOptions::BackgroundMode::Vignette;
            optionVignetteBackground();
            break;
        }
        case 2: {
            m_meshLoadingScene.getOptions().backgroundMode = MeshLoadingSceneOptions::BackgroundMode::Gradient;
            optionGradientBackground();
            break;
        }
        default: break;
    }
    ImGui::Unindent(indent);

    // Camera modes
    std::vector<const char*> cameraModes = {"Fixed", "Cylinder", "Turntable", "Trackball", "Model mover"};
    static int currentCameraMode = 0;
    ImGui::Combo("Camera Mode", &currentCameraMode, cameraModes.data(), static_cast<int>(cameraModes.size()));
    ImGui::Indent(indent);
    switch(currentCameraMode)
    {
        case 0: {
            m_meshLoadingScene.getOptions().cameraMode = MeshLoadingSceneOptions::CameraMode::Fixed;
            break;
        }
        case 1: {
            m_meshLoadingScene.getOptions().cameraMode = MeshLoadingSceneOptions::CameraMode::Cylinder;
            break;
        }
        case 2: {
            m_meshLoadingScene.getOptions().cameraMode = MeshLoadingSceneOptions::CameraMode::Turntable;
            break;
        }
        case 3: {
            m_meshLoadingScene.getOptions().cameraMode = MeshLoadingSceneOptions::CameraMode::Trackball;
            break;
        }
        case 4: {
            m_meshLoadingScene.getOptions().cameraMode = MeshLoadingSceneOptions::CameraMode::ModelMover;
            break;
        }
        default: break;
    }
    ImGui::Unindent(indent);

    // Infinite grid display toggle
    ImGui::Checkbox("Display Infinite Grid", &m_meshLoadingScene.getOptions().displayGrid);
    ImGui::Indent(indent);
    optionInfiniteGridBackground();
    ImGui::Unindent(indent);

    ImGui::End();
}

void EditorLayer::optionSingleColorBackground()
{
    ImGui::ColorEdit3("Background Color", (float*)&m_meshLoadingScene.getOptions().backgroundColor);
}

void EditorLayer::optionVignetteBackground()
{
    ImGui::ColorEdit3("Inner Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorVignetteInner);
    ImGui::ColorEdit3("Outer Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorVignetteOuter);
    ImGui::SliderFloat("Vignette Radius", &m_meshLoadingScene.getOptions().vignetteRadius, 0.0f, 1.0f);
    ImGui::SliderFloat("Vignette Softness", &m_meshLoadingScene.getOptions().vignetteSoftness, 0.0f, 1.0f);
}

void EditorLayer::optionGradientBackground()
{
    ImGui::ColorEdit3("Top Left Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorTopLeft);
    ImGui::ColorEdit3("Top Right Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorTopRight);
    ImGui::ColorEdit3("Bottom Left Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorBottomLeft);
    ImGui::ColorEdit3("Bottom Right Color", (float*)&m_meshLoadingScene.getOptions().backgroundColorBottomRight);
}

void EditorLayer::optionInfiniteGridBackground() {}

bool EditorLayer::isCoordInViewport(glm::vec2 coord)
{
    coord.y = m_layerHeight - coord.y;
    return (coord.x > m_viewportData.x && coord.x < m_viewportData.x + m_viewportData.width &&
            coord.y > m_viewportData.y && coord.y < m_viewportData.y + m_viewportData.height);
}

bool EditorLayer::onFileDropped(core::FileDropEvent& event)
{
    if(event.getPaths().empty())
    {
        core::Logger::logWarning("No file dropped.");
        return true;
    }

    auto mousePos = core::Input::getMousePosition();
    if(isCoordInViewport(mousePos))
    {
        if(event.getPaths().size() > 1)
        {
            core::Logger::logWarning("Multiple files dropped. Only the first one will be loaded.");
        }
        m_meshLoadingScene.loadMeshFromFile(event.getPaths().front());
        return true;
    }
    return false;
}

void EditorLayer::displayViewportWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{1, 1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Viewport");

    auto& vMin = m_viewportData.m_vMin;
    auto& vMax = m_viewportData.m_vMax;

    vMin = ImGui::GetWindowContentRegionMin();
    vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;

    m_viewportData.x = vMin.x;
    m_viewportData.y = m_layerHeight - vMax.y;
    m_viewportData.width = vMax.x - vMin.x;
    m_viewportData.height = vMax.y - vMin.y;

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}