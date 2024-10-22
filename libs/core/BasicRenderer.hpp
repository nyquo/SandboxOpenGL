#pragma once

#include "CoreExport.hpp"
#include "Renderer.hpp"

namespace core {

class CORE_API BasicRenderer : public Renderer
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
    std::unique_ptr<core::Shader> m_modelShader;
};
}
