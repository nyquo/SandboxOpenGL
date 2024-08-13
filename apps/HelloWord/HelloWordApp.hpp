#pragma once

#include <Application.hpp>
#include <Events/Event.hpp>
#include <Layer.hpp>
#include <Logger.hpp>
#include <Shader.hpp>

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void onEvent(core::Event& event) override {}

  private:
    float vertices[9]{-0.5F, -0.5F, 0.0F, 0.0F, 0.5F, 0.0F, 0.5F, -0.5F, 0.0F};
    unsigned int VBO, VAO;
    std::unique_ptr<core::Shader> shaderProgram;
};

class HelloWordApp : public core::Application
{
  public:
    HelloWordApp() { getWindow().setLayer(std::make_unique<CustomLayer>()); }
};
