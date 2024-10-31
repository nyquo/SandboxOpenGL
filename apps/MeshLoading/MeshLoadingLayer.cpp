#include "MeshLoadingLayer.hpp"

#include <Conversion.hpp>
#include <Logger.hpp>
#include <Window.hpp>

MeshLoadingLayer::MeshLoadingLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(std::make_shared<renderer::PerspectiveCamera>(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F)))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)
{
    m_scene.addModel(std::make_shared<renderer::Model>(
      std::filesystem::path(std::string(RESSOURCES_FOLDER) + "/assets/GravelyPlane/GravelyPlane.obj")));
}

MeshLoadingLayer::~MeshLoadingLayer() {}

void MeshLoadingLayer::onUpdate()
{
    processInputs();
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    updateData();
    m_renderer.beginFrame();
    m_renderer.renderScene(m_scene, m_camera);
    m_renderer.endFrame();
}

void MeshLoadingLayer::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(MeshLoadingLayer::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(MeshLoadingLayer::onMouseMoved));
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

    ImGui::Text("Directional light");
    ImGui::DragFloat3("Direction", (float*)&m_guiData.m_directionalLight.m_direction);
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

    ImGui::Checkbox("Enable Info Overlay", &m_guiData.m_enableOverlayInfo);
    ImGui::Checkbox("Enable MSAA", &m_guiData.m_enableMSAA);
    if(ImGui::Button("Exit app"))
    {
        glfwSetWindowShouldClose(m_window->getWindow(), GLFW_TRUE);
    }
    ImGui::End();

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
}

void MeshLoadingLayer::setViewportSize(float viewportWidth, float viewportHeight)
{
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_camera->setViewPortSize(viewportWidth, viewportHeight);
}

void MeshLoadingLayer::setCameraMovement(bool cameraMovementEnabled)
{
    m_cameraMovementEnabled = cameraMovementEnabled;
}

void MeshLoadingLayer::updateData()
{
    renderer::DirectionalLight light;
    m_scene.setDirectionalLight(m_guiData.m_directionalLight);
    m_scene.clearPointLights();
    m_scene.setPointLightVec(m_guiData.m_pointLights);

    // TDOO CLEANUP THIS AS SOON AS POSSIBLE
    auto sceneModels = m_scene.getModels();
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
        m_scene.addModel(model);

        ModelData mData;
        mData.m_name = model->getName();
        m_guiData.m_models.push_back(mData);
    }
    else
    {
        core::Logger::logError("Error loading model");
    }
}

void MeshLoadingLayer::setShowUi(bool showUi) { m_showUi = showUi; }

void MeshLoadingLayer::processInputs()
{
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    if(m_cameraMovementEnabled)
    {
        const float cameraSpeed = 10.0F * m_deltaTime;
        glm::vec3 positionOffset{0.0F, 0.0F, 0.0F};

        if(isKeyPressed(GLFW_KEY_W))
        {
            positionOffset.z += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_S))
        {
            positionOffset.z -= cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_A))
        {
            positionOffset.x -= cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_D))
        {
            positionOffset.x += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_SPACE))
        {
            positionOffset.y += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        {
            positionOffset.y -= cameraSpeed;
        }
        m_camera->translateCameraRelative(positionOffset);
    }
}

bool MeshLoadingLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    if(m_cameraMovementEnabled)
    {
        m_camera->adustFov(-m_zoomOffset * e.getYOffset());
    }
    return true;
}

bool MeshLoadingLayer::onMouseMoved(core::MouseMovedEvent& e)
{
    if(m_firstMouse)
    {
        m_lastMouseX = e.getX();
        m_lastMouseY = e.getY();
        m_firstMouse = false;
    }
    float xOffset = e.getX() - m_lastMouseX;
    float yOffset = m_lastMouseY - e.getY();
    m_lastMouseX = e.getX();
    m_lastMouseY = e.getY();
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    if(m_cameraMovementEnabled)
    {
        m_camera->rotateCamera(xOffset, yOffset);
    }

    return true;
}