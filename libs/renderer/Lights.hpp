#pragma once

#include "RendererExport.hpp"

#include <glm/glm.hpp>

namespace renderer {

struct RENDERER_API DirectionalLight
{
    DirectionalLight() = default;
    DirectionalLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
      : m_direction(direction)
      , m_ambientColor(ambientColor)
      , m_diffuseColor(diffuseColor)
      , m_specularColor(specularColor)
    {}

    glm::vec3 m_direction;
    glm::vec3 m_ambientColor;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;
};

struct RENDERER_API PointLight
{
    PointLight() = default;
    PointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
      : m_position(position)
      , m_ambientColor(ambientColor)
      , m_diffuseColor(diffuseColor)
      , m_specularColor(specularColor)
    {}

    glm::vec3 m_position;

    glm::vec3 m_ambientColor;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;

    float m_constant{1.0f};
    float m_linear{0.09f};
    float m_quadratic{0.032f};
};

}
