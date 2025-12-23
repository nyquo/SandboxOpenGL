#pragma once

#include <core/gl.h>

namespace renderer {

struct FrameBufferSpecification
{
    unsigned int width{800};
    unsigned int height{600};
};

class FrameBuffer
{
  public:
    FrameBuffer(const FrameBufferSpecification& frameBufferSpec);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&& other) noexcept;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&& other) noexcept;
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void resize(unsigned int width, unsigned int height);

  private:
    unsigned int m_frameBufferId{0};
    unsigned int m_textureColorBufferId{0};
    unsigned int m_renderBufferId{0};

    FrameBufferSpecification m_specification;
};

}