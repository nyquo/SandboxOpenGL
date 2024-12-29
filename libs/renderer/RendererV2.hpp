#pragma once

// #include "Renderer.hpp"
#include "RendererExport.hpp"
#include "SceneV2.hpp"
#include "Shader.hpp"
#include "Shapes/Square.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace renderer {

struct ViewportData
{
    int x{0};
    int y{0};
    int width{800};
    int height{600};
};

class RENDERER_API RendererV2
{
  public:
    RendererV2();
    RendererV2(const RendererV2& other) = delete;
    RendererV2(RendererV2&& other) = delete;
    RendererV2& operator=(const RendererV2& other) = delete;
    RendererV2& operator=(RendererV2&& other) = delete;
    ~RendererV2();

    void beginFrame();
    void renderScene(SceneV2& scene);
    void endFrame();
    void setViewport(int x, int y, int width, int height);

  private:
    ViewportData m_viewportData;
    std::unique_ptr<Shader> m_simulatedEntityShader;
    renderer::Square m_square;
};

}
