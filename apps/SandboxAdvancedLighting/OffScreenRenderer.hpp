#pragma once

#include "Quad.hpp"

#include <renderer/Buffers.hpp>
#include <renderer/FrameBuffer.hpp>
#include <renderer/Shader.hpp>

class OffScreenRenderer
{
  public:
    OffScreenRenderer(unsigned int textureWidth, unsigned int textureHeight);
    ~OffScreenRenderer();
    OffScreenRenderer(const OffScreenRenderer&) = delete;
    OffScreenRenderer(OffScreenRenderer&& other) = delete;
    OffScreenRenderer& operator=(const OffScreenRenderer&) = delete;
    OffScreenRenderer& operator=(OffScreenRenderer&& other) = delete;

    void resize(unsigned int textureWidth, unsigned int textureHeight);
    void setGammaCorrection(float gamma) { m_gammaCorrection = gamma; }

    void bindFrameBuffer() { m_frameBuffer.bind(); }
    void unbindFrameBuffer() { m_frameBuffer.unbind(); }

    void draw();

  private:
    Quad m_quad{};

    renderer::Shader m_screenShader;
    renderer::FrameBuffer m_frameBuffer;

    float m_gammaCorrection{1.9f};
};