#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <renderer/Buffers.hpp>
#include <renderer/Model.hpp>
#include <renderer/PerspectiveCamera.hpp>
#include <renderer/Shader.hpp>

namespace fs = std::filesystem;

struct MeshLoadingSceneOptions
{
    enum class BackgroundMode
    {
        SingleColor = 0,
        Vignette = 1
    };

    BackgroundMode backgroundMode{BackgroundMode::SingleColor};

    // Single color settings
    glm::vec3 backgroundColor{0.15f, 0.15f, 0.15f};

    // Vignette settings
    glm::vec3 backgroundColorVignetteInner{0.56f, 0.56f, 0.56f};
    glm::vec3 backgroundColorVignetteOuter{0.31f, 0.31f, 0.31f};
    float vignetteRadius{1.0f};
    float vignetteSoftness{0.9f};

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
    void renderBackground();
    void renderModel();
    void beginFrame();
    void endFrame();

  private:
    int m_x{0};
    int m_y{0};
    int m_width{800};
    int m_height{600};

    std::shared_ptr<renderer::Model> m_model;
    std::shared_ptr<renderer::PerspectiveCamera> m_camera;

    std::shared_ptr<renderer::Shader> m_basicModelShader;
    std::shared_ptr<renderer::Shader> m_backgroundVignetteShader;

    std::unique_ptr<renderer::VertexBuffer> m_quadVBO;
    std::unique_ptr<renderer::VertexArray> m_quadVAO;
    std::unique_ptr<renderer::IndexBuffer> m_quadIBO;
    std::shared_ptr<renderer::Shader> m_currentBackgroundShader;

    MeshLoadingSceneOptions m_options;
};