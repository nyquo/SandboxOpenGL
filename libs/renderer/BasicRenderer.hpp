#pragma once

#include "Renderer.hpp"
#include "RendererExport.hpp"

namespace renderer {

class RENDERER_API BasicRenderer : public Renderer
{
  public:
    BasicRenderer();
    BasicRenderer(const BasicRenderer&) = delete;
    BasicRenderer(BasicRenderer&&) = delete;
    BasicRenderer& operator=(const BasicRenderer&) = delete;
    BasicRenderer& operator=(BasicRenderer&&) = delete;
    ~BasicRenderer() = default;

    void beginFrame() override;
    void renderScene(const Scene& scene, std::shared_ptr<Camera> camera) const override;
    void endFrame() override;
    void setViewport(int width, int height, int x = 0, int y = 0) override;

  private:
    std::unique_ptr<Shader> m_modelShader;
};
}
