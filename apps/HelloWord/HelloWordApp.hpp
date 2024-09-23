#pragma once

#include <Application.hpp>
#include <Events/Event.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/WindowEvent.hpp>
#include <Layer.hpp>
#include <Logger.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CustomLayer : public core::Layer
{
  public:
    CustomLayer()
    {
        shaderProgram = std::make_unique<core::Shader>("shaders/BasicShader.vert", "shaders/BasicShader.frag");

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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

    ~CustomLayer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void onUpdate() override
    {
        glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

        shaderProgram->bind();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

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
        shaderProgram->setMat4("model", model);
        shaderProgram->setMat4("view", view);
        shaderProgram->setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void onEvent(core::Event& e) override
    {
        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(CustomLayer::onMouseScrolled));
        dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(CustomLayer::onMouseMoved));
    }
    int m_windowWidth;
    int m_windowHeight;

  private:
    bool onMouseScrolled(core::MouseScrolledEvent& e)
    {
        factor = std::max(0.0f, std::min(1.0f, factor + (step * e.getYOffset())));
        return false;
    }
    bool onMouseMoved(core::MouseMovedEvent& e)
    {
        m_mousePosXNorm = ((e.getX() * 2) / m_windowWidth) - 1;
        m_mousePosYNorm = -((e.getY() * 2) / m_windowHeight) + 1;
        return false;
    }

  private:

    float vertices[180] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;

    std::unique_ptr<core::Shader> shaderProgram;
    std::unique_ptr<core::Texture> texture1, texture2;

    float factor{0.5};
    float step{0.05};
    float m_mousePosXNorm, m_mousePosYNorm;
};

class HelloWordApp : public core::Application
{
  public:
    HelloWordApp()
    {
        getWindow().setLayer(std::make_shared<CustomLayer>());

        auto customLayer = std::dynamic_pointer_cast<CustomLayer>(getWindow().getLayer());

        customLayer->m_windowHeight = getWindow().getHeight();
        customLayer->m_windowWidth = getWindow().getWidth();
    }

    void onEvent(core::Event& e) override
    {
        core::Application::onEvent(e);

        core::EventDispatcher dispatcher(e);
        dispatcher.dispatch<core::WindowResizeEvent>(BIND_EVENT_FN(HelloWordApp::onWindowResized));
    }

    bool onWindowResized(core::WindowResizeEvent& e)
    {
        auto customLayer = std::dynamic_pointer_cast<CustomLayer>(getWindow().getLayer());

        customLayer->m_windowHeight = e.getHeight();
        customLayer->m_windowWidth = e.getWidth();

        return false;
    }
};
