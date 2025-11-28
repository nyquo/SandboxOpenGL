#include "InstanceRenderingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
InstanceRenderingScene::InstanceRenderingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Instance Rendering", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_quadShader(std::string(RESSOURCES_FOLDER) + "/shaders/ColoredQuad.vert",
                 std::string(RESSOURCES_FOLDER) + "/shaders/ColoredQuad.frag")
  , m_quadVBO(sizeof(float) * (2 + 3) * 6, nullptr)
  , m_instanceVBO(sizeof(glm::vec2) * 100, nullptr)
{
    // clang-format off
    float quadVertices[] = {
    // positions     // colors
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f	
    };
    // clang-format on
    m_quadVBO.setData(quadVertices, sizeof(quadVertices));
    renderer::BufferLayout layout{
      renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float)), // position
      renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float))  // color
    };
    m_quadVBO.setLayout(std::move(layout));
    m_quadVAO.addVertexBuffer(m_quadVBO);

    glm::vec2 translations[100];

    int index = 0;

    float quadSize = 0.1f;
    float quadPerSide = 10.0f;
    float spacing = (2.0f - (quadSize * quadPerSide)) / quadPerSide;
    float margin = spacing * 0.5f;

    for(float y = -1.0 + margin; y <= 1.0 - quadSize - margin; y += quadSize + spacing)
    {
        for(float x = -1.0 + margin; x <= 1.0 - quadSize - margin; x += quadSize + spacing)
        {
            glm::vec2 translation;
            translation.x = x + quadSize * 0.5f;
            translation.y = y + quadSize * 0.5f;
            translations[index++] = translation;
        }
    }

    m_instanceVBO.setData(translations, sizeof(translations));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
}

void InstanceRenderingScene::onEvent(core::Event& event) {}

void InstanceRenderingScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    // m_camera.setViewPortSize(getWidth(), getHeight());

    begin();
    drawScene();
    end();
}

void InstanceRenderingScene::drawScene()
{
    m_quadShader.bind();
    m_quadVAO.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}