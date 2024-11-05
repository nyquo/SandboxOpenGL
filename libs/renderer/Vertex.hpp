#pragma once

#include "RendererExport.hpp"

#include <glm/glm.hpp>

namespace renderer {

struct RENDERER_API Vertex
{
    Vertex(glm::vec3 _pos = {}, glm::vec3 _norm = {}, glm::vec2 _texCoords = {})
      : position(_pos)
      , normal(_norm)
      , texCoords(_texCoords)
    {}

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

}
