#pragma once

#include <core/gl.h>

#include <vector>

namespace renderer {

struct BufferElement
{
    BufferElement(unsigned int dataType, size_t count, bool normalize, size_t size)
      : m_dataType(dataType)
      , m_count(count)
      , m_normalize(normalize)
      , m_size(size)
      , m_offset(0)
    {}
    unsigned int m_dataType;
    size_t m_count;
    bool m_normalize;
    size_t m_size;
    size_t m_offset;
};

class BufferLayout
{
  public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    const std::vector<BufferElement>& getElements() const { return m_elements; }
    size_t getStride() const { return m_stride; }

  private:
    std::vector<BufferElement> m_elements;
    size_t m_stride{0};
};

class VertexBuffer
{
  public:
    VertexBuffer(size_t size, void* data);
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    const BufferLayout& getLayout() const { return m_layout; }
    void setLayout(BufferLayout&& layout);

  private:
    unsigned int m_id{0};
    BufferLayout m_layout;
};

class IndexBuffer
{
  public:
    IndexBuffer(size_t count, unsigned int* indices);
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    size_t getCount() const { return m_count; }

  private:
    unsigned int m_id{0};
    size_t m_count;
};

class VertexArray
{
  public:
    VertexArray();
    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void addVertexBuffer(VertexBuffer& vertexBuffer);
    void setIndexBuffer(IndexBuffer& indexBuffer);

    void bind() const;
    void unbind() const;

  private:
    unsigned int m_id{0};
};

}
