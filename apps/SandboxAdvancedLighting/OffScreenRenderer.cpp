#include "OffScreenRenderer.hpp"

#include "core/gl.h"

#include <glm/glm.hpp>
#include <renderer/Buffers.hpp>
OffScreenRenderer::OffScreenRenderer(unsigned int textureWidth, unsigned int textureHeight)
  : m_frameBuffer({textureWidth, textureHeight})
  , m_screenShader(std::string(RESSOURCES_FOLDER) + "/shaders/screen.vert",
                   std::string(RESSOURCES_FOLDER) + "/shaders/screen.frag")
{
    renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float)),
                                  renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float))};
    m_quadVBO.setLayout(std::move(layout));
    m_quadVAO.addVertexBuffer(m_quadVBO);

    // clang-format off
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    // clang-format on
    m_quadVBO.setData(quadVertices, sizeof(quadVertices));
}

OffScreenRenderer::~OffScreenRenderer() {}

void OffScreenRenderer::resize(unsigned int width, unsigned int height) { m_frameBuffer.resize(width, height); }

void OffScreenRenderer::draw()
{
    // default frame buffer should already be bound when calling this function
    m_screenShader.bind();
    m_screenShader.setFloat("gamma", m_gammaCorrection);
    m_quadVAO.bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, m_frameBuffer.getColorAttachmentRendererId());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}