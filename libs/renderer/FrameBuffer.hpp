#pragma once

#include <core/gl.h>

namespace renderer {

enum class DepthAttachmentType
{
    None = 0,
    RenderBuffer = 1,
    Texture = 2
};

// May be too simple, will see later
struct FrameBufferSpecification
{
    unsigned int width{800};
    unsigned int height{600};
    bool hasColorAttachment{true};
    DepthAttachmentType depthAttachmentType{DepthAttachmentType::RenderBuffer};
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

    unsigned int getColorAttachmentId() const;

  private:
    unsigned int m_frameBufferId{0};

    unsigned int m_colorTextureBufferId{0};
    unsigned int m_depthTextureBufferId{0};
    unsigned int m_depthRenderBufferId{0};

    FrameBufferSpecification m_specification;
};

}