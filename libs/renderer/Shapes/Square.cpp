#include "Square.hpp"

namespace renderer {

Square::Square()
  : m_vertexBuffer(sizeof(float) * m_vertices.size(), m_vertices.data())
  , m_indexBuffer(m_indices.size(), m_indices.data())
{
    BufferLayout layout{BufferElement(GL_FLOAT, 2, false, sizeof(float))};
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);
}

size_t Square::getIndicesCount() const { return m_indices.size(); }

const IndexBuffer& Square::getIndexBuffer() const { return m_indexBuffer; }

const VertexArray& Square::getVertexArray() const { return m_vertexArray; }

}
