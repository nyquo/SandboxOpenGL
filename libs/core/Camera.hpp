#include <glm/glm.hpp>

namespace core {

class Camera
{
  public:
    Camera(const glm::mat4& projection = glm::mat4(1.0F), const glm::mat4& view = glm::mat4(1.0F))
      : m_projection(projection)
      , m_view(view)
    {}
    virtual ~Camera() = default;

    [[nodiscard]] glm::mat4 getProjection() const { return m_projection; }
    [[nodiscard]] glm::mat4 getView() const { return m_view; }

  protected:
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

}
