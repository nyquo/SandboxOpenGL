#pragma once

#include "RendererExport.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

namespace renderer {

class RENDERER_API Entity
{
  public:
    Entity() = default;
    Entity(const Entity& other) = delete;
    Entity(Entity&& other) = delete;
    Entity operator=(const Entity& other) = delete;
    Entity operator=(Entity&& other) = delete;
    virtual ~Entity() = default;

    virtual void draw(Shader& shader) const = 0;

    glm::mat4 getModelMat() const { return m_modelMat; };
    void setModelMat(const glm::mat4& modelMat) { m_modelMat = modelMat; }

  protected:
    glm::mat4 m_modelMat{1.0f};

    // temp
  public:
    bool outline{false};
};

}
