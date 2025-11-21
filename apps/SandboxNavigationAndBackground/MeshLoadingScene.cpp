#include "MeshLoadingScene.hpp"

#include "CylinderCameraMover.hpp"
#include "TrackballCameraMover.hpp"
#include "TurntableCameraMover.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/Model.hpp>

MeshLoadingScene::MeshLoadingScene()
{
    m_basicModelShader =
      std::make_shared<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BasicModelShader.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/BasicModelShader.frag");
    m_backgroundVignetteShader =
      std::make_shared<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundVignette.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundVignette.frag");
    m_backgroundGradientShader =
      std::make_shared<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundGradient.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundGradient.frag");
    m_infiniteGridShader =
      std::make_shared<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundInfiniteGrid.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/BackgroundInfiniteGrid.frag");

    m_camera = std::make_shared<renderer::PerspectiveCamera>(m_width, m_height, glm::vec3(0.0F, 3.0F, 10.0F));
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));

    float quadBackground[] = {-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f};
    unsigned int quadIndices[] = {0, 2, 1, 2, 0, 3};
    m_quadVBO = std::make_unique<renderer::VertexBuffer>(sizeof(float) * 8, quadBackground);
    m_quadIBO = std::make_unique<renderer::IndexBuffer>(6, quadIndices);
    m_quadVAO = std::make_unique<renderer::VertexArray>();
    renderer::BufferLayout quadLayout{renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float))};
    m_quadVBO->setLayout(std::move(quadLayout));
    m_quadVAO->addVertexBuffer(*m_quadVBO);
    m_quadVAO->setIndexBuffer(*m_quadIBO);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void MeshLoadingScene::onEvent(core::Event& event)
{
    if(m_cameraMover)
    {
        m_cameraMover->onEvent(event);
    }
}

void MeshLoadingScene::update()
{
    if(m_options.cameraMode != m_options.oldCameraMode)
    {
        switch(m_options.cameraMode)
        {
            case MeshLoadingSceneOptions::CameraMode::Fixed: {
                m_cameraMover = nullptr;
                break;
            }
            case MeshLoadingSceneOptions::CameraMode::Cylinder: {
                m_cameraMover = std::make_shared<CylinderCameraMover>(m_camera);
                m_cameraMover->init();
                break;
            }
            case MeshLoadingSceneOptions::CameraMode::Turntable: {
                m_cameraMover = std::make_shared<TurntableCameraMover>(m_camera);
                m_cameraMover->init();
                break;
            }
            case MeshLoadingSceneOptions::CameraMode::Trackball: {
                m_cameraMover = std::make_shared<TrackballCameraMover>(m_camera);
                m_cameraMover->init();
                break;
            }
        }
        m_options.oldCameraMode = m_options.cameraMode;
    }
    if(m_cameraMover)
    {
        m_cameraMover->update();
    }
    renderScene();
}

MeshLoadingScene::~MeshLoadingScene() {}

void MeshLoadingScene::renderScene()
{
    beginFrame();
    glDisable(GL_DEPTH_TEST);
    renderBackground();
    renderInfiniteGrid();
    glEnable(GL_DEPTH_TEST);
    renderModel(); // TODO render background afterwards? May need to tweek zindex handeling
    endFrame();
}

void MeshLoadingScene::renderBackground()
{
    switch(m_options.backgroundMode)
    {
        case MeshLoadingSceneOptions::BackgroundMode::SingleColor: {
            setupSingleColorBackground();
            break;
        }
        case MeshLoadingSceneOptions::BackgroundMode::Vignette: {
            setupVignetteBackground();
            break;
        }
        case MeshLoadingSceneOptions::BackgroundMode::Gradient: {
            setupGradientBackground();
            break;
        }
    }

    if(m_currentBackgroundShader)
    {
        m_quadVAO->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void MeshLoadingScene::renderModel()
{
    if(m_model)
    {
        m_basicModelShader->bind();

        m_basicModelShader->setMat4("view", m_camera->getView());
        m_basicModelShader->setMat4("projection", m_camera->getProjection());
        m_basicModelShader->setVec3("viewPos", m_camera->getPosition());
        m_basicModelShader->setMat4("model", glm::mat4(1.0f));

        m_model->draw(*m_basicModelShader);
    }
}

void MeshLoadingScene::beginFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glViewport(m_x, m_y, m_width, m_height);
    glScissor(m_x, m_y, m_width, m_height);

    const auto& color = m_options.backgroundMode == MeshLoadingSceneOptions::BackgroundMode::SingleColor
                          ? m_options.backgroundColor
                          : glm::vec3(0.0f);
    glClearColor(color.r, color.g, color.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MeshLoadingScene::endFrame() { glDisable(GL_SCISSOR_TEST); }

void MeshLoadingScene::setupSingleColorBackground() { m_currentBackgroundShader = nullptr; }

void MeshLoadingScene::setupVignetteBackground()
{
    m_currentBackgroundShader = m_backgroundVignetteShader;
    m_currentBackgroundShader->bind();
    m_currentBackgroundShader->setVec3("innerColor", m_options.backgroundColorVignetteInner);
    m_currentBackgroundShader->setVec3("outerColor", m_options.backgroundColorVignetteOuter);
    m_currentBackgroundShader->setFloat("radius", m_options.vignetteRadius);
    m_currentBackgroundShader->setFloat("softness", m_options.vignetteSoftness);
}

void MeshLoadingScene::setupGradientBackground()
{
    m_currentBackgroundShader = m_backgroundGradientShader;
    m_currentBackgroundShader->bind();
    m_currentBackgroundShader->setVec3("topLeftColor", m_options.backgroundColorTopLeft);
    m_currentBackgroundShader->setVec3("topRightColor", m_options.backgroundColorTopRight);
    m_currentBackgroundShader->setVec3("bottomLeftColor", m_options.backgroundColorBottomLeft);
    m_currentBackgroundShader->setVec3("bottomRightColor", m_options.backgroundColorBottomRight);
}

void MeshLoadingScene::renderInfiniteGrid()
{
    if(m_options.displayGrid)
    {
        m_infiniteGridShader->bind();
        m_infiniteGridShader->setMat4("projection", m_camera->getProjection());
        m_infiniteGridShader->setMat4("view", m_camera->getView());

        renderer::VertexArray emptyVAO;
        emptyVAO.bind(); // A non-zero Vertex Array Object must be bound
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void MeshLoadingScene::setViewport(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_camera->setViewPortSize(width, height);
}

void MeshLoadingScene::loadMeshFromFile(fs::path path)
{
    m_model = nullptr;
    if(path.empty())
    {
        core::Logger::logWarning("Removing previous mesh (no path provided).");
        return;
    }
    else
    {
        core::Logger::logInfo("Loading mesh from path: ", path);
        m_model = std::make_shared<renderer::Model>(path);
    }
}
