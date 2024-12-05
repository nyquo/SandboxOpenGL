#pragma once

#include <core/gl.h>

#include <vector>

namespace renderer {

struct VBLayoutElement
{
    VBLayoutElement(unsigned int _dataType, size_t _size, bool _normalize)
      : dataType(_dataType)
      , size(_size)
      , normalize(_normalize)
    {}
    unsigned int dataType;
    size_t size;
    bool normalize;
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

    const std::vector<VBLayoutElement>& getLayout() const { return m_layout; }
    void setLayout(std::vector<VBLayoutElement>&& layout);

  private:
    unsigned int m_id{0};
    std::vector<VBLayoutElement> m_layout;
};

class IndexBuffer
{
  public:
    IndexBuffer(size_t size, void* data);
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

  private:
    unsigned int m_id{0};
};

// TODO Remove raw pointers
class VertexArray
{
  public:
    VertexArray();
    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void setVertexBuffer(VertexBuffer* vertexBuffer);
    void setIndexBuffer(IndexBuffer* indexBuffer);

    void bind() const;
    void unbind() const;

  private:
    unsigned int m_id{0};
    VertexBuffer* m_vertexBuffer{nullptr};
    IndexBuffer* m_indexBuffer{nullptr};
};

}
