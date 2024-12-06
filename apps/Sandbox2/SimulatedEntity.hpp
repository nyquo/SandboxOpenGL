#pragma once

#include <glm/glm.hpp>

class SimulatedEntity
{
  public:
    SimulatedEntity() {}

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_color{1.0f, 0.0f, 1.0f};
};
