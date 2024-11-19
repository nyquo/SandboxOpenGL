#pragma once

#include "Camera.hpp"
#include "CoreExport.hpp"
#include "Scene.hpp"

#include <memory>

namespace renderer {

class CORE_API Renderer
{
  public:
    Renderer() = default;
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer operator=(const Renderer& other) = delete;
    Renderer operator=(Renderer&& other) = delete;
    virtual ~Renderer() = default;

    virtual void beginFrame() = 0;
    virtual void renderScene(const Scene& scene, std::shared_ptr<Camera> camera) = 0;
    virtual void endFrame() = 0;
    virtual void setViewport(int width, int height, int x = 0, int y = 0) = 0;

    // TODO set render target ? screen / image / buffer ?
    // TODO set render type ? wireframe ?
};

}
