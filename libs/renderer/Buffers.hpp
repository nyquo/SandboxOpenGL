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

}
