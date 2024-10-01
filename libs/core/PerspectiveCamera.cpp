
#include "PerspectiveCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace core {

PerspectiveCamera::PerspectiveCamera(
  float viewportWidth, float viewPortHeight, glm::vec3 position, float yaw, float pitch)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewPortHeight)
  , m_position(position)
  , m_yaw(yaw)
  , m_pitch(pitch)
{
    updateView();
    updateProjection();
}

void PerspectiveCamera::setFov(float fov)
{
    m_fov = std::max(m_minFov, std::min(m_maxFov, fov));
    updateProjection();
}

void PerspectiveCamera::adustFov(float offset) { setFov(m_fov + offset); }

void PerspectiveCamera::rotateCamera(float xOffset, float yOffset)
{
    m_yaw += xOffset;
    m_pitch += yOffset;

    if(m_constraintPitch)
    {
        m_pitch = std::max(-89.0F, std::min(89.0F, m_pitch));
    }

    updateView();
}

void PerspectiveCamera::translateCamera(glm::vec3 translation)
{
    m_position += translation;
    updateView();
}

void PerspectiveCamera::translateCameraRelative(glm::vec3 translation)
{
    m_position += (translation.z * m_front);
    m_position += (translation.x * glm::normalize(glm::cross(m_front, m_up)));
    m_position.y += translation.y;
    updateView();
}

void PerspectiveCamera::setViewPortSize(float m_width, float m_height)
{
    m_viewportWidth = m_width;
    m_viewportHeight = m_height;
    updateProjection();
}

void PerspectiveCamera::updateView()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(direction);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void PerspectiveCamera::updateProjection()
{
    m_projection = glm::perspective(glm::radians(m_fov), m_viewportWidth / m_viewportHeight, m_near, m_far);
}

}
