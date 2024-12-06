#include "Square.hpp"

namespace renderer {

Square::Square()
  : m_vertexBuffer(sizeof(float) * m_vertices.size(), m_vertices.data())
  , m_indexBuffer(sizeof(unsigned int) * m_indices.size(), m_indices.data())
{
    std::vector<VBLayoutElement> layout;
    layout.emplace_back(GL_FLOAT, 3, false);
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.setVertexBuffer(&m_vertexBuffer);
    m_vertexArray.setIndexBuffer(&m_indexBuffer);
}

size_t Square::getIndicesCount() const { return m_indices.size(); }

const IndexBuffer& Square::getIndexBuffer() const { return m_indexBuffer; }

const VertexArray& Square::getVertexArray() const { return m_vertexArray; }

}
