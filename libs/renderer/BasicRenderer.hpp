#pragma once

#include "BasicCube.hpp"
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
    void renderScene(const Scene& scene, std::shared_ptr<Camera> camera) override;
    void endFrame() override;
    void setViewport(int width, int height, int x = 0, int y = 0) override;
    void setWireFrame(bool wireFrame) { m_wireFrame = wireFrame; }

  private:
    void renderSceneOnscreen(const Scene& scene, const std::shared_ptr<Camera> camera);
    void renderSceneOffscren(const Scene& scene, const std::shared_ptr<Camera> camera);

    void initOffscreenRendering();
    void deinitOffscreenRendering();

  private:
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_outlineShader;

    // temp TO DELETE
    mutable BasicCube m_cube;

    bool m_wireFrame{false};
    bool m_offscreen{false};

    unsigned int m_frameBuffer{0};
};

}
