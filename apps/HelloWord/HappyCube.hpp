#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

class HappyCube
{
  public:
    HappyCube();
    ~HappyCube();

    const float* getRawVertices() const;
    size_t getVertexCount() const;
    glm::mat4 getModelMatrix() const;
    void setModelMatrix(glm::mat4 modelMatrix);

  private:
    const std::array<float, 180> m_vertices{
      -0.5F, -0.5F, -0.5F, 0.0F, 0.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 0.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
      0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 0.0F,

      -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
      0.5F,  0.5F,  0.5F,  1.0F, 1.0F, -0.5F, 0.5F,  0.5F,  0.0F, 1.0F, -0.5F, -0.5F, 0.5F,  0.0F, 0.0F,

      -0.5F, 0.5F,  0.5F,  1.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 1.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
      -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  0.5F,  1.0F, 0.0F,

      0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 0.0F, 1.0F,
      0.5F,  -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

      -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 1.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F,
      0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,

      -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
      0.5F,  0.5F,  0.5F,  1.0F, 0.0F, -0.5F, 0.5F,  0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F};

    glm::mat4 m_modelMatrix{1.0f};
};
