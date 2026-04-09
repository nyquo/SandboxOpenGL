#include "OffScreenRenderer.hpp"

#include "core/gl.h"

#include <glm/glm.hpp>
#include <renderer/Buffers.hpp>

OffScreenRenderer::OffScreenRenderer(unsigned int textureWidth, unsigned int textureHeight)
  : m_frameBuffer({textureWidth, textureHeight})
  , m_screenShader(std::string(RESSOURCES_FOLDER) + "/shaders/screen.vert",
                   std::string(RESSOURCES_FOLDER) + "/shaders/screen.frag")
{
    m_screenShader.bind();
}

OffScreenRenderer::~OffScreenRenderer() {}

void OffScreenRenderer::resize(unsigned int width, unsigned int height) { m_frameBuffer.resize(width, height); }

void OffScreenRenderer::draw()
{
    // default frame buffer should already be bound when calling this function
    m_screenShader.bind();
    m_screenShader.setFloat("gamma", m_gammaCorrection);
    m_quad.bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, m_frameBuffer.getColorAttachmentId());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}