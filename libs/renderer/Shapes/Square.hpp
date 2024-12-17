#pragma once

#include "renderer/Buffers.hpp"

#include <vector>

namespace renderer {

class Square
{
  public:
    Square();
    Square(const Square& other) = delete;
    Square(Square&& other) = delete;
    Square& operator=(const Square& other) = delete;
    Square& operator=(Square&& other) = delete;

    const renderer::VertexArray& getVertexArray() const;
    const renderer::IndexBuffer& getIndexBuffer() const;
    size_t getIndicesCount() const;

  private:
    std::vector<float> m_vertices{
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
    };
    std::vector<unsigned int> m_indices{0, 1, 2, 3, 4, 5};

    renderer::VertexBuffer m_vertexBuffer;
    renderer::IndexBuffer m_indexBuffer;
    renderer::VertexArray m_vertexArray;
};

}
