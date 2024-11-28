#pragma once

#include <gl.h>

namespace renderer {

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

  private:
    unsigned int m_id{0};
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

}
