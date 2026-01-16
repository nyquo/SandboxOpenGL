#include "FrameBuffer.hpp"

#include "core/Logger.hpp"
#include "core/gl.h"

namespace renderer {

FrameBuffer::FrameBuffer(const FrameBufferSpecification& frameBufferSpec)
  : m_specification(frameBufferSpec)
{
    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    if(frameBufferSpec.hasColorAttachment)
    {
        glGenTextures(1, &m_colorTextureBufferId);
        glBindTexture(GL_TEXTURE_2D, m_colorTextureBufferId);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     m_specification.width,
                     m_specification.height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureBufferId, 0);
    }
    else
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    if(m_specification.depthAttachmentType == DepthAttachmentType::Texture)
    {
        glGenTextures(1, &m_depthTextureBufferId);
        glBindTexture(GL_TEXTURE_2D, m_depthTextureBufferId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH24_STENCIL8,
                     m_specification.width,
                     m_specification.height,
                     0,
                     GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureBufferId, 0);
    }
    else if(m_specification.depthAttachmentType == DepthAttachmentType::RenderBuffer)
    {
        glGenRenderbuffers(1, &m_depthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        core::Logger::logError("Frame buffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_frameBufferId);
    if(m_specification.hasColorAttachment)
    {
        glDeleteTextures(1, &m_colorTextureBufferId);
    }
    if(m_specification.depthAttachmentType == DepthAttachmentType::RenderBuffer)
    {
        glDeleteRenderbuffers(1, &m_depthRenderBufferId);
    }
    else if(m_specification.depthAttachmentType == DepthAttachmentType::Texture)
    {
        glDeleteTextures(1, &m_depthTextureBufferId);
    }
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glViewport(0, 0, m_specification.width, m_specification.height);
}

void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
    if(m_specification.width == width && m_specification.height == height)
    {
        return;
    }

    m_specification.width = width;
    m_specification.height = height;

    if(m_specification.hasColorAttachment)
    {
        glBindTexture(GL_TEXTURE_2D, m_colorTextureBufferId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     m_specification.width,
                     m_specification.height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if(m_specification.depthAttachmentType == DepthAttachmentType::Texture)
    {
        glBindTexture(GL_TEXTURE_2D, m_depthTextureBufferId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH24_STENCIL8,
                     m_specification.width,
                     m_specification.height,
                     0,
                     GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8,
                     nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if(m_specification.depthAttachmentType == DepthAttachmentType::RenderBuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

unsigned int FrameBuffer::getColorAttachmentId() const
{
    if(!m_specification.hasColorAttachment)
    {
        core::Logger::logError("FrameBuffer has no color attachment");
    }
    return m_colorTextureBufferId;
}

}