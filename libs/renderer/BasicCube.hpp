#pragma once

#include "Buffers.hpp"

#include <Shader.hpp>
#include <gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>
// TEMP TO DELETE
class BasicCube
{
  public:
    BasicCube(glm::vec3 position = glm::vec3(0.0F))
      : m_position(position)
      , m_vertexBuffer(sizeof(float) * m_vertices.size(), m_vertices.data())
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        m_shader = std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.vert",
                                                      std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.frag");
    }

    ~BasicCube()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vao);
    }

    const float* getRawVertices() const { return m_vertices.data(); }

    size_t getVertexCount() const { return m_vertices.size(); }

  private:
    std::vector<float> m_vertices{-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
                                  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0};

  public:
    glm::mat4 m_modelMatrix{1.0F};
    glm::vec3 m_position;
    glm::vec3 m_rotationVector;

    renderer::VertexBuffer m_vertexBuffer;
    unsigned int m_vao;

    std::unique_ptr<renderer::Shader> m_shader;
};
