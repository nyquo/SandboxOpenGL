#include "ViewportLayer.hpp"

#include <Conversion.hpp>
#include <Logger.hpp>

ViewportLayer::ViewportLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(std::make_shared<renderer::PerspectiveCamera>(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F)))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)
{
    m_scene.addModel(std::make_shared<renderer::Model>(
      std::filesystem::path(std::string(RESSOURCES_FOLDER) + "/assets/GravelyPlane/GravelyPlane.obj")));
}

ViewportLayer::~ViewportLayer() {}

void ViewportLayer::onUpdate()
{
    processInputs();
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    m_renderer.beginFrame();
    m_renderer.renderScene(m_scene, m_camera);
    m_renderer.endFrame();
}

void ViewportLayer::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(ViewportLayer::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(ViewportLayer::onMouseMoved));
}

void ViewportLayer::setViewportSize(float viewportWidth, float viewportHeight)
{
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_camera->setViewPortSize(viewportWidth, viewportHeight);
}

void ViewportLayer::setCameraMovement(bool cameraMovementEnabled) { m_cameraMovementEnabled = cameraMovementEnabled; }

void ViewportLayer::setGuiData(const GuiData& guiData)
{
    renderer::DirectionalLight light;
    m_scene.setDirectionalLight(guiData.m_directionalLight);
    m_scene.clearPointLights();
    m_scene.setPointLightVec(guiData.m_pointLights);

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

    m_guiData = guiData;
}

void ViewportLayer::loadModel(std::shared_ptr<renderer::Model> model) { m_scene.addModel(model); }

void ViewportLayer::processInputs()
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

bool ViewportLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    if(m_cameraMovementEnabled)
    {
        m_camera->adustFov(-m_zoomOffset * e.getYOffset());
    }
    return true;
}

bool ViewportLayer::onMouseMoved(core::MouseMovedEvent& e)
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
