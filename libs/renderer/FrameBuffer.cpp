#include "FrameBuffer.hpp"

#include "core/Logger.hpp"
#include "core/gl.h"

namespace renderer {

FrameBuffer::FrameBuffer(const FrameBufferSpecification& frameBufferSpec)
  : m_specification(frameBufferSpec)
{
    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    glGenTextures(1, &m_textureColorBufferId);
    glBindTexture(GL_TEXTURE_2D, m_textureColorBufferId);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, m_specification.width, m_specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBufferId, 0);

    glGenRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        core::Logger::logError("Frame buffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept {}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {}

FrameBuffer::~FrameBuffer() {}

void FrameBuffer::bind() const {}

void FrameBuffer::unbind() const {}

void FrameBuffer::resize(unsigned int width, unsigned int height) {}

}