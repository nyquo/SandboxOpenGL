#pragma once

#include <glm/glm.hpp>
#include <random>

class SimulatedEntity
{
  public:
    SimulatedEntity()
    {
        static std::mt19937 gen{std::random_device{}()};
        static std::uniform_real_distribution<float> distPosition(-40.0F, 40.0F);
        static std::uniform_real_distribution<float> distColor(0.0F, 1.0F);
        m_position = {distPosition(gen), distPosition(gen), 0.0f};
        m_color = {distColor(gen), distColor(gen), distColor(gen)};
    }

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_color{0.5f, 0.5f, 0.5f};
};
