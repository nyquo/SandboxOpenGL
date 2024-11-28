#include "Buffers.hpp"

namespace renderer {

VertexBuffer::VertexBuffer(size_t size, void* data)
{
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
  : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

VertexBuffer::~VertexBuffer()
{
    if(m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
    }
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

IndexBuffer::IndexBuffer(size_t size, void* data)
{
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
  : m_id(other.m_id)
{
    other.m_id = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

IndexBuffer::~IndexBuffer()
{
    if(m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
    }
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}
