#pragma once

#include <glm/glm.hpp>

class SimulatedEntity
{
  public:
    SimulatedEntity() {}

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_color{0.5f, 0.5f, 0.5f};
};
