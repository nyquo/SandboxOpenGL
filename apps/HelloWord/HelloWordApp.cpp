#include "HelloWordApp.hpp"

#include <random>

CustomLayer::CustomLayer()
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

    shaderProgram = std::make_unique<core::Shader>("shaders/BasicShader.vert", "shaders/BasicShader.frag");

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

    texture1 = std::make_unique<core::Texture>("assets/container.jpg", GL_TEXTURE0);
    texture2 = std::make_unique<core::Texture>("assets/awesomeface.png", GL_TEXTURE1);

    glBindVertexArray(0);
};

CustomLayer::~CustomLayer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void CustomLayer::onProcessInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    const float cameraSpeed = 2.5f * m_deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cameraPos += cameraSpeed * m_cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cameraPos -= cameraSpeed * m_cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    }
}

void CustomLayer::onUpdate()
{
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(fov), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);

    texture1->bind();
    texture2->bind();

    shaderProgram->setInt("texture1", 0);
    shaderProgram->setInt("texture2", 1);

    shaderProgram->setMat4("view", view);
    shaderProgram->setMat4("projection", projection);

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

bool CustomLayer::onKeyPressed(core::KeyPressedEvent& e) { return false; }

bool CustomLayer::onKeyReleased(core::KeyReleasedEvent& e) { return false; }
