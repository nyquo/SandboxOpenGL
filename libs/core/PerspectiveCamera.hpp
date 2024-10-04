#include "Camera.hpp"

namespace core {

class PerspectiveCamera : public Camera
{
  public:
    PerspectiveCamera(float viewportWidth = 0,
                      float viewPortHeight = 0,
                      glm::vec3 position = glm::vec3{0.0F, 0.0F, 0.0F},
                      float yaw = -90,
                      float pitch = 0.0F);

    void setFov(float fov);
    void adustFov(float offset);

    void rotateCamera(float xOffset, float yOffset);
    void translateCamera(glm::vec3 translation);
    void translateCameraRelative(glm::vec3 translation);

    void setViewPortSize(float m_width, float m_height);

    glm::vec3 getPosition();
    glm::vec3 getDirection();

  private:
    void updateView();
    void updateProjection();

  private:
    glm::vec3 m_position;
    glm::vec3 m_front{0.0F, 0.0F, -1.0F};
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp{0.0F, 1.0F, 0.0F};

    float m_yaw;
    float m_pitch;

    float m_near{0.1};
    float m_far{100};

    float m_fov{45.0};
    float m_minFov{0.0F};
    float m_maxFov{110.0F};

    float m_viewportWidth;
    float m_viewportHeight;

    bool m_constraintPitch{false};
};

}
