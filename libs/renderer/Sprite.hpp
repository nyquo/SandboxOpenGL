#pragma once

#include "Entity.hpp"
#include "RendererExport.hpp"
#include "Shader.hpp"

namespace renderer {

class RENDERER_API Sprite : public Entity
{
  public:
    Sprite() = default;
    Sprite(const Sprite& other) = delete;
    Sprite(Sprite&& other) = delete;
    Sprite operator=(const Sprite& other) = delete;
    Sprite operator=(Sprite&& other) = delete;
    ~Sprite() = default;

    void draw(Shader& shader) const final;

  private:
};

}
