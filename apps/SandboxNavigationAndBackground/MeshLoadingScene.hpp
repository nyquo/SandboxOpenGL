#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Model.hpp>

namespace fs = std::filesystem;

struct MeshLoadingSceneOptions
{
    glm::vec3 backgroundColor{0.0f, 0.1f, 0.1f};
    char meshPath[256] = "";
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
    void loadMeshFromFile(fs::path path);

  private:
    void beginFrame();
    void endFrame();

  private:
    int m_x{0};
    int m_y{0};
    int m_width{800};
    int m_height{600};

    std::shared_ptr<renderer::Model> m_model;

    MeshLoadingSceneOptions m_options;
};