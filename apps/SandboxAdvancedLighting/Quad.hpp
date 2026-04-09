#pragma once

#include <renderer/Buffers.hpp>

class Quad
{
  public:
    Quad()
    {
        renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float)),
                                      renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float))};
        m_quadVBO.setLayout(std::move(layout));
        m_quadVAO.addVertexBuffer(m_quadVBO);

        // clang-format off
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
        // clang-format on
        m_quadVBO.setData(quadVertices, sizeof(quadVertices));
    }
    ~Quad() = default;
    Quad(const Quad&) = delete;
    Quad(Quad&& other) = delete;
    Quad& operator=(const Quad&) = delete;
    Quad& operator=(Quad&& other) = delete;

    void bind() { m_quadVAO.bind(); }

  private:
    renderer::VertexBuffer m_quadVBO{6 * 2 * 2 * sizeof(float)}; // 6 vertices with 2 pos and 2 tex coords
    renderer::VertexArray m_quadVAO;
};