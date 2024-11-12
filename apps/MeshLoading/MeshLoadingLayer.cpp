#include "MeshLoadingLayer.hpp"

#include <Conversion.hpp>
#include <Input.hpp>
#include <Logger.hpp>
#include <Sprite.hpp>
#include <Window.hpp>

MeshLoadingLayer::MeshLoadingLayer(float layerWidth, float layerHeight)
  : m_layerWidth(layerWidth)
  , m_layerHeight(layerHeight)
  , m_camera(std::make_shared<renderer::PerspectiveCamera>(layerWidth, layerHeight, glm::vec3(0.0F, 0.0F, 10.0F)))
{
    m_scene.addEntity(std::make_shared<renderer::Model>(
      std::filesystem::path(std::string(RESSOURCES_FOLDER) + "/assets/GravelyPlane/GravelyPlane.obj")));
}

MeshLoadingLayer::~MeshLoadingLayer() {}

void MeshLoadingLayer::onUpdate()
{
    m_fpsCameraMover.update();
    processInputs();
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    updateData();
    m_renderer.setViewport(m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMin.x, m_layerHeight - m_vMax.y);
    m_camera->setViewPortSize(m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y);
    m_renderer.beginFrame();
    m_renderer.renderScene(m_scene, m_camera);
    m_renderer.endFrame();
}

void MeshLoadingLayer::onEvent(core::Event& e)
{
    m_fpsCameraMover.onEvent(e);
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(MeshLoadingLayer::onKeyPressed));
    dispatcher.dispatch<core::MouseButtonPressedEvent>(BIND_EVENT_FN(MeshLoadingLayer::onMouseButtonPressed));
}

void MeshLoadingLayer::onImGuiRender()
{
    if(!m_showUi)
    {
        return;
    }

    ImGui::Begin("Options Window");

    ImGui::InputText("Model path", m_guiData.m_modelPath, IM_ARRAYSIZE(m_guiData.m_modelPath));

    if(ImGui::Button("Load model"))
    {
        loadModel(fs::path(m_guiData.m_modelPath));
    }

    ImGui::InputText("Sprite path", m_guiData.m_spritePath, IM_ARRAYSIZE(m_guiData.m_spritePath));

    if(ImGui::Button("Load sprite"))
    {
        loadSprite(fs::path(m_guiData.m_spritePath));
    }

    ImGui::Text("Directional light");
    ImGui::DragFloat3("Direction", (float*)&m_guiData.m_directionalLight.m_direction, .01);
    ImGui::ColorEdit3("D Ambient light color", (float*)&m_guiData.m_directionalLight.m_ambientColor);
    ImGui::ColorEdit3("D Diffuse light color", (float*)&m_guiData.m_directionalLight.m_diffuseColor);
    ImGui::ColorEdit3("D Specular light color", (float*)&m_guiData.m_directionalLight.m_specularColor);

    if(ImGui::Button("Add Point light"))
    {
        m_guiData.m_pointLights.emplace_back(renderer::PointLight(glm::vec3(0.0f, 1.0f, 0.0f),
                                                                  glm::vec3(0.2f, 0.2f, 0.2f),
                                                                  glm::vec3(0.5f, 0.5f, 0.5f),
                                                                  glm::vec3(1.0f, 1.0f, 1.0)));
    }

    for(int i = 0; i < m_guiData.m_pointLights.size(); ++i)
    {
        std::string name = "Point Light " + std::to_string(i);
        if(ImGui::TreeNode(name.c_str()))
        {
            ImGui::DragFloat3("position", (float*)&m_guiData.m_pointLights[i], 0.01f);
            ImGui::ColorEdit3("D Ambient light color", (float*)&m_guiData.m_pointLights[i].m_ambientColor);
            ImGui::ColorEdit3("D Diffuse light color", (float*)&m_guiData.m_pointLights[i].m_diffuseColor);
            ImGui::ColorEdit3("D Specular light color", (float*)&m_guiData.m_pointLights[i].m_specularColor);
            if(ImGui::Button("remove"))
            {
                m_guiData.m_pointLights.erase(std::next(m_guiData.m_pointLights.begin(), i));
            }
            ImGui::TreePop();
        }
    }

    for(int i = 0; i < m_guiData.m_models.size(); ++i)
    {
        std::string name = m_guiData.m_models[i].m_name + std::to_string(i);
        if(ImGui::TreeNode(name.c_str()))
        {
            ImGui::DragFloat3("position", (float*)&m_guiData.m_models[i].m_position, 0.01f);
            ImGui::Checkbox("Outline", &m_guiData.m_models[i].m_outline);
            ImGui::TreePop();
        }
    }

    ImGui::Checkbox("Enable Info Overlay", &m_guiData.m_displayOverlayInfo);
    ImGui::Checkbox("Enable MSAA", &m_guiData.m_enableMSAA);
    ImGui::Checkbox("Wireframe", &m_guiData.m_wireFrame);
    if(ImGui::Button("Exit app"))
    {
        if(m_onExit)
        {
            m_onExit();
        }
    }

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{1, 1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Begin("Viewport"); //, nullptr, ImGuiWindowFlags_NoBackground);

    m_vMin = ImGui::GetWindowContentRegionMin();
    m_vMax = ImGui::GetWindowContentRegionMax();

    m_vMin.x += ImGui::GetWindowPos().x;
    m_vMin.y += ImGui::GetWindowPos().y;
    m_vMax.x += ImGui::GetWindowPos().x;
    m_vMax.y += ImGui::GetWindowPos().y;

    // ImGui::Text(std::to_string(m_vMin.y).c_str());
    // ImGui::Text(std::to_string(m_vMax.y).c_str());

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void MeshLoadingLayer::setLayerSize(float width, float height)
{
    m_layerWidth = width;
    m_layerHeight = height;
}

void MeshLoadingLayer::updateData()
{
    if(m_guiData.m_enableMSAA != m_guiData.m_oldEnablerMSAA)
    {
        m_guiData.m_oldEnablerMSAA = m_guiData.m_enableMSAA;
        if(m_guiData.m_enableMSAA)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
    }

    if(m_guiData.m_displayOverlayInfo != m_guiData.m_oldDisplayOverlayInfo)
    {
        m_guiData.m_oldDisplayOverlayInfo = m_guiData.m_displayOverlayInfo;
        if(m_onDisplayOverlayChanged)
        {
            m_onDisplayOverlayChanged(m_guiData.m_displayOverlayInfo);
        }
    }

    if(m_guiData.m_wireFrame != m_guiData.m_oldWireFrame)
    {
        m_guiData.m_oldWireFrame = m_guiData.m_wireFrame;
        m_renderer.setWireFrame(m_guiData.m_wireFrame);
    }

    renderer::DirectionalLight light;
    m_scene.setDirectionalLight(m_guiData.m_directionalLight);
    m_scene.clearPointLights();
    m_scene.setPointLightVec(m_guiData.m_pointLights);

    auto sceneModels = m_scene.getEntities();
    for(int i = 1; i < sceneModels.size(); ++i)
    {
        if(m_guiData.m_models.size() > i - 1)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_guiData.m_models.at(i - 1).m_position);
            sceneModels[i]->outline = m_guiData.m_models.at(i - 1).m_outline;
            sceneModels[i]->setModelMat(model);
        }
    }
}

void MeshLoadingLayer::loadModel(fs::path path)
{
    std::shared_ptr<renderer::Model> model = std::make_shared<renderer::Model>(path);
    core::Logger::logInfo("Loading model from path: ", path);
    if(model->getName() != "")
    {
        m_scene.addEntity(model);

        ModelData mData;
        mData.m_name = model->getName();
        m_guiData.m_models.push_back(mData);
    }
    else
    {
        core::Logger::logError("Error loading model");
    }
}

void MeshLoadingLayer::loadSprite(fs::path path)
{
    std::shared_ptr<renderer::Sprite> sprite = std::make_shared<renderer::Sprite>(path);
    core::Logger::logInfo("Loading sprite from path: ", path);

    m_scene.addEntity(sprite);

    ModelData mData;
    mData.m_outline = false;
    mData.m_name = sprite->getName();
    m_guiData.m_models.push_back(mData);
}

void MeshLoadingLayer::processInputs() {}

bool MeshLoadingLayer::onKeyPressed(core::KeyPressedEvent& e)
{
    if(e.getKeyCode() == GLFW_KEY_ESCAPE)
    {
        m_fpsCameraMover.disable();
        auto* currentWindow = glfwGetCurrentContext();
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return false;
}

bool MeshLoadingLayer::onMouseButtonPressed(core::MouseButtonPressedEvent& e)
{
    // If click is in viewport
    auto mousePos = core::Input::getMousePosition();
    if(mousePos.x >= m_vMin.x && mousePos.x <= m_vMax.x && mousePos.y >= m_layerHeight - m_vMax.y &&
       mousePos.y <= m_layerHeight - m_vMin.y)
    {
        auto* currentWindow = glfwGetCurrentContext();
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_fpsCameraMover.enable();
    }

    return false;
}

void MeshLoadingLayer::setDisplayOverlayChangedCallBack(std::function<void(bool)> callBack)
{
    m_onDisplayOverlayChanged = callBack;
}

void MeshLoadingLayer::setExitCallBack(std::function<void()> callBack) { m_onExit = callBack; }
