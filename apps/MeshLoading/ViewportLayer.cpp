#include "ViewportLayer.hpp"

#include <Conversion.hpp>
#include <Logger.hpp>

ViewportLayer::ViewportLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)
{
    m_modelShader = std::make_unique<core::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                                   std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag");
}

ViewportLayer::~ViewportLayer() {}

void ViewportLayer::onUpdate()
{
    processInputs();

    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_model)
    {
        m_modelShader->bind();

        m_modelShader->setMat4("view", m_camera.getView());
        m_modelShader->setMat4("projection", m_camera.getProjection());
        m_modelShader->setMat4("model", glm::mat4(1.0f));

        m_model->draw(*m_modelShader);
    }
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
    m_camera.setViewPortSize(viewportWidth, viewportHeight);
}

void ViewportLayer::setCameraMovement(bool cameraMovementEnabled) { m_cameraMovementEnabled = cameraMovementEnabled; }

void ViewportLayer::setGuiData(const GuiData& guiData) { m_guiData = guiData; }

void ViewportLayer::loadModel()
{
    core::Logger::logInfo("Loading model from path: " + std::string(m_guiData.m_modelPath));
    m_model.reset(new core::Model(std::filesystem::path(m_guiData.m_modelPath)));
}

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
        m_camera.translateCameraRelative(positionOffset);
    }
}

bool ViewportLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    if(m_cameraMovementEnabled)
    {
        m_camera.adustFov(-m_zoomOffset * e.getYOffset());
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
        m_camera.rotateCamera(xOffset, yOffset);
    }

    return true;
}