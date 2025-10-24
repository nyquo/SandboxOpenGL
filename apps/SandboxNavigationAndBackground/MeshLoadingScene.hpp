#pragma once

#include <glm/glm.hpp>

struct MeshLoadingSceneOptions
{
    glm::vec3 backgroundColor{0.0f, 0.1f, 0.1f};
};

class MeshLoadingScene
{
  public:
    ~MeshLoadingScene();
    MeshLoadingScene(const MeshLoadingScene& other) = delete;
    MeshLoadingScene(MeshLoadingScene&& other) = delete;
    MeshLoadingScene& operator=(const MeshLoadingScene& other) = delete;
    MeshLoadingScene& operator=(MeshLoadingScene&& other) = delete;
    MeshLoadingScene();

    void renderScene();
    void setViewport(int x, int y, int width, int height);
    MeshLoadingSceneOptions& getOptions() { return m_options; }

  private:
    void beginFrame();
    void endFrame();

  private:
    int m_x{0};
    int m_y{0};
    int m_width{800};
    int m_height{600};

    MeshLoadingSceneOptions m_options;
};