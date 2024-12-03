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
    ~BasicRenderer();

    void beginFrame() override;
    void renderScene(const Scene& scene, std::shared_ptr<Camera> camera) override;
    void endFrame() override;
    void setViewport(int width, int height, int x = 0, int y = 0) override;
    void setWireFrame(bool wireFrame) { m_wireFrame = wireFrame; }

  private:
    void renderSceneOnscreen(const Scene& scene, const std::shared_ptr<Camera> camera);
    void renderSceneOffscren(const Scene& scene, const std::shared_ptr<Camera> camera);
    void renderSceneImpl(const Scene& scene, const std::shared_ptr<Camera> camera);

    void initOffscreenRendering();
    void deinitOffscreenRendering();

  private:
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_outlineShader;
    std::unique_ptr<Shader> m_screenShader;

    int m_viewportX{0};
    int m_viewportY{0};
    int m_viewportWidth{200};
    int m_viewportHeight{200};

    // temp TO DELETE
    mutable BasicCube m_cube;

    bool m_wireFrame{false};
    bool m_offscreen{false};

    unsigned int m_frameBuffer{0};
    unsigned int m_textureColorBuffer{0};
    unsigned int m_rbo{0};

    std::unique_ptr<VertexBuffer> m_offscreenQuadVB;
    std::unique_ptr<IndexBuffer> m_offscreenQuadIB;
    std::unique_ptr<VertexArray> m_offscreenQuadVA;
    unsigned int m_offscreenTextureWidth{1920};
    unsigned int m_offscreenTextureHeight{1080};
};

}
