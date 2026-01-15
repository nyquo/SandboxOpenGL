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
    FrameBuffer(FrameBuffer&& other) = delete; // I will implement move when needed
    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&& other) = delete; // I will implement move when needed
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void resize(unsigned int width, unsigned int height);

    unsigned int getColorAttachmentRendererId() const { return m_textureColorBufferId; }

  private:
    unsigned int m_frameBufferId{0};
    unsigned int m_textureColorBufferId{0};
    unsigned int m_renderBufferId{0};

    FrameBufferSpecification m_specification;
};

}