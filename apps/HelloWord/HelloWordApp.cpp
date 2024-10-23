#include "HelloWordApp.hpp"

#include <random>

CustomLayer::CustomLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)

{
    for(int i = 0; i < 16; ++i)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 80);
        m_happyCubes.emplace_back(
          glm::vec3(((float)dist(rng) - 40) / 10, ((float)dist(rng) - 40) / 10, ((float)dist(rng) - 40) / 10));
    }

    for(auto& cube : m_happyCubes)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);

        cube.m_rotationVector = glm::vec3(((float)dist(rng)) / 100, ((float)dist(rng)) / 100, ((float)dist(rng)) / 100);
    }

    shaderProgram = std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.vert",
                                                       std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.frag");

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(m_happyCubes[0].getRawVertices()) * m_happyCubes[0].getVertexCount(),
                 m_happyCubes[0].getRawVertices(),
                 GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture1 =
      std::make_unique<renderer::Texture>(std::string(RESSOURCES_FOLDER) + "/assets/container.jpg", "texture_diffuse");
    glActiveTexture(GL_TEXTURE0);
    texture2 = std::make_unique<renderer::Texture>(std::string(RESSOURCES_FOLDER) + "/assets/awesomeface.png",
                                                   "texture_diffuse");
    glActiveTexture(GL_TEXTURE1);

    glBindVertexArray(0);
};

CustomLayer::~CustomLayer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void CustomLayer::onUpdate()
{
    processInputs();

    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1->getId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2->getId());

    shaderProgram->setInt("texture1", 0);
    shaderProgram->setInt("texture2", 1);

    shaderProgram->setMat4("view", m_camera.getView());
    shaderProgram->setMat4("projection", m_camera.getProjection());

    glBindVertexArray(VAO);
    for(auto& cube : m_happyCubes)
    {
        cube.m_modelMatrix = glm::mat4(1.0F);
        cube.m_modelMatrix = glm::translate(cube.m_modelMatrix, cube.m_position);

        cube.m_modelMatrix = glm::rotate(cube.m_modelMatrix, (float)glfwGetTime(), cube.m_rotationVector);
        shaderProgram->setMat4("model", cube.m_modelMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void CustomLayer::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(CustomLayer::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(CustomLayer::onMouseMoved));
    dispatcher.dispatch<core::KeyPressedEvent>(BIND_EVENT_FN(CustomLayer::onKeyPressed));
    dispatcher.dispatch<core::KeyReleasedEvent>(BIND_EVENT_FN(CustomLayer::onKeyReleased));
}

void CustomLayer::setViewportSize(float viewportWidth, float viewportHeight)
{
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_camera.setViewPortSize(viewportWidth, viewportHeight);
}

void CustomLayer::processInputs()
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
    m_camera.translateCameraRelative(positionOffset);
}

bool CustomLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    m_camera.adustFov(-m_zoomOffset * e.getYOffset());
    return true;
}

bool CustomLayer::onMouseMoved(core::MouseMovedEvent& e)
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

bool CustomLayer::onKeyPressed(core::KeyPressedEvent& e) { return false; }

bool CustomLayer::onKeyReleased(core::KeyReleasedEvent& e) { return false; }
