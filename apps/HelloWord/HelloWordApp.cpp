#include "HelloWordApp.hpp"

CustomLayer::CustomLayer()
{
    shaderProgram = std::make_unique<core::Shader>("shaders/BasicShader.vert", "shaders/BasicShader.frag");

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(m_happyCube.getRawVertices()) * m_happyCube.getVertexCount(),
                 m_happyCube.getRawVertices(),
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

void CustomLayer::onUpdate()
{
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    m_happyCube.setModelMatrix(glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f)));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);

    texture1->bind();
    texture2->bind();

    shaderProgram->setInt("texture1", 0);
    shaderProgram->setInt("texture2", 1);

    shaderProgram->setFloat("factor", factor);
    shaderProgram->setMat4("model", m_happyCube.getModelMatrix());
    shaderProgram->setMat4("view", view);
    shaderProgram->setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
