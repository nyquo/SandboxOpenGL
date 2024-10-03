#include "ViewportLayer.hpp"

ViewportLayer::ViewportLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)
{
    m_shaderProgram = std::make_unique<core::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.vert",
                                                     std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.frag");

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(m_cube.getRawVertices()) * m_cube.getVertexCount(),
                 m_cube.getRawVertices(),
                 GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    m_cube.m_modelMatrix = glm::mat4(1.0F);
    m_cube.m_position = glm::vec3(0.0F, 0.0F, 0.0F);
    m_cube.m_modelMatrix = glm::translate(m_cube.m_modelMatrix, m_cube.m_position);
}

ViewportLayer::~ViewportLayer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ViewportLayer::onUpdate()
{
    processInputs();

    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram->bind();

    m_shaderProgram->setMat4("view", m_camera.getView());
    m_shaderProgram->setMat4("projection", m_camera.getProjection());
    m_shaderProgram->setMat4("model", m_cube.m_modelMatrix);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);
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

void ViewportLayer::processInputs()
{
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
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

bool ViewportLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    m_camera.adustFov(-m_zoomOffset * e.getYOffset());
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
    m_camera.rotateCamera(xOffset, yOffset);

    return true;
}
