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
  , m_layout(std::move(other.m_layout))
{
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    m_id = other.m_id;
    m_layout = std::move(other.m_layout);
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

void VertexBuffer::setLayout(std::vector<VBLayoutElement>&& layout) { m_layout = std::move(layout); }

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

VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::VertexArray(VertexArray&& other) noexcept
  : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

VertexArray::~VertexArray()
{
    if(m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
    }
}

void VertexArray::setVertexBuffer(VertexBuffer* vertexBuffer)
{
    m_vertexBuffer = vertexBuffer;
    bind();
    if(m_vertexBuffer != nullptr)
    {
        size_t index = 0;
        size_t stride = 0;
        size_t offset = 0;
        for(const auto& element : m_vertexBuffer->getLayout())
        {
            stride += sizeof(element.dataType) * element.size;
        }
        for(const auto& element : m_vertexBuffer->getLayout())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.size, element.dataType, element.normalize, stride, (void*)offset);
            ++index;
            offset += sizeof(element.dataType) * element.size;
        }
    }
}

void VertexArray::setIndexBuffer(IndexBuffer* indexBuffer)
{
    bind();
    m_indexBuffer = indexBuffer;
    if(m_indexBuffer != nullptr)
    {
        indexBuffer->bind();
    }
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

}
