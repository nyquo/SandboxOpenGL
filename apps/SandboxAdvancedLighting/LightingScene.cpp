#include "LightingScene.hpp"

#include "OffScreenRenderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui.h"
#include "renderer/FrameBuffer.hpp"

#include <core/Input.hpp>
#include <core/gl.h>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <random>

LightingScene::LightingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Lighting Scene", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_camera(
      std::make_shared<renderer::PerspectiveCamera>(getLayerWidth(), getLayerHeight(), glm::vec3(0.0F, 0.0F, 10.0F)))
  , m_shader(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag")
  , m_lightCubeShader(std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.vert",
                      std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.frag",
                      std::string(RESSOURCES_FOLDER) + "/shaders/pointLightCube.geom")
  , m_depthShader(std::string(RESSOURCES_FOLDER) + "/shaders/simpleDepthShader.vert",
                  std::string(RESSOURCES_FOLDER) + "/shaders/simpleDepthShader.frag")
  , m_cameraMover(m_camera)
  , m_offScreenRenderer(800, 600) // initial size, will be resized later
  , m_shadowsFB({1024, 1024, false, renderer::DepthAttachmentType::Texture})
{
    renderer::Texture::setGammaCorrectionEnabled(true);
    m_model = std::make_unique<renderer::Model>(std::string(RESSOURCES_FOLDER) + "/models/Barrel/Barrel.obj", false);
    initFloorMesh();
    randomizeModelsPositions();
    m_cameraMover.init();

    // Setup point light VBO and VAO
    updatePointLightBuffer();
    renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                                  renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float))};
    m_pointLightsVBO.setLayout(std::move(layout));
    m_pointLightsVAO.addVertexBuffer(m_pointLightsVBO);

    float near{1.0f};
    float far{10.0f};
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
}

void LightingScene::onEvent(core::Event& event) { m_cameraMover.onEvent(event); }

void LightingScene::onImGuiRender()
{
    ImGui::Begin("Lighting Scene Settings");
    ImGui::Checkbox("Blinn-Phong", &m_blinnPhong);
    ImGui::Checkbox("Display shadow frame buffer", &m_displayShadowFB);
    if(ImGui::DragFloat("Gamma Correction", &m_gammaCorrection, 0.01f, 1.0f, 5.0f))
    {
        m_offScreenRenderer.setGammaCorrection(m_gammaCorrection);
    }
    if(ImGui::Button("New seed"))
    {
        m_gen.seed(m_rd());
        randomizeModelsPositions();
    }
    ImGui::Text("Material settings");
    float shininess = m_floorMesh->getShininess();
    if(ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 128.0f))
    {
        m_floorMesh->setShininess(shininess);
    }
    ImGui::NewLine();
    ImGui::Text("Directional Light Settings");
    ImGui::DragFloat3("Direction", (float*)&m_directionalLight.m_direction, .01);
    ImGui::ColorEdit3("D Ambient light color", (float*)&m_directionalLight.m_ambientColor);
    ImGui::ColorEdit3("D Diffuse light color", (float*)&m_directionalLight.m_diffuseColor);
    ImGui::ColorEdit3("D Specular light color", (float*)&m_directionalLight.m_specularColor);
    ImGui::NewLine();
    ImGui::Text("Point Light Settings");
    ImGui::DragFloat3("Point light position", (float*)&m_pointLight.m_position, 0.01f);
    ImGui::ColorEdit3("Point light ambient color", (float*)&m_pointLight.m_ambientColor);
    ImGui::ColorEdit3("Point light diffuse color", (float*)&m_pointLight.m_diffuseColor);
    ImGui::ColorEdit3("Point light specular color", (float*)&m_pointLight.m_specularColor);
    ImGui::End();
}

void LightingScene::initFloorMesh()
{
    const float floorHalfSize = 10.0f;
    std::vector<renderer::Vertex> floorVertices = {
      // positions            // normals         // texture coords
      {{floorHalfSize, 0.0f, floorHalfSize}, {0.0f, 1.0f, 0.0f}, {10.0f, 10.0f}},
      {{-floorHalfSize, 0.0f, -floorHalfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{-floorHalfSize, 0.0f, floorHalfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
      {{floorHalfSize, 0.0f, -floorHalfSize}, {0.0f, 1.0f, 0.0f}, {10.0f, 0.0f}},
    };

    std::vector<unsigned int> floorIndices = {
      0,
      1,
      2,
      0,
      3,
      1,
    };

    std::vector<renderer::Texture> floorTextures;
    floorTextures.emplace_back(std::string(RESSOURCES_FOLDER) + "/textures/wood.png", "texture_diffuse");

    m_floorMesh =
      std::make_unique<renderer::Mesh>(std::move(floorVertices), std::move(floorIndices), std::move(floorTextures));
}

void LightingScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    m_camera->setViewPortSize(getWidth(), getHeight());
    auto mousePosition = core::Input::getMousePosition();
    m_cameraMover.setIsMouseInViewport(isInViewport(mousePosition.x, mousePosition.y));
    m_cameraMover.update();

    // render to depth map
    renderDepthMap();

    if(m_displayShadowFB)
    {
        displayShadowFrameBuffer();
    }
    else
    {
        // draw to framebuffer
        m_offScreenRenderer.resize(static_cast<unsigned int>(getWidth()), static_cast<unsigned int>(getHeight()));
        m_offScreenRenderer.bindFrameBuffer();
        drawScene();
        m_offScreenRenderer.unbindFrameBuffer();

        // draw framebuffer texture to screen
        begin();
        m_offScreenRenderer.draw();
        end();
    }
}

void LightingScene::drawScene()
{
    // See if the clearing should be elsewhere
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    auto projectionView = m_camera->getProjection() * m_camera->getView();

    m_shader.bind();

    m_shader.setBool("blinn", m_blinnPhong);

    m_shader.setMat4("projection", m_camera->getProjection());
    m_shader.setMat4("view", m_camera->getView());
    m_shader.setMat4("model", glm::mat4(1.0f));
    m_shader.setVec3("viewPos", m_camera->getPosition());

    // Directional light
    m_shader.setVec3("dirLight.direction", m_directionalLight.m_direction);
    m_shader.setVec3("dirLight.ambient", m_directionalLight.m_ambientColor);
    m_shader.setVec3("dirLight.diffuse", m_directionalLight.m_diffuseColor);
    m_shader.setVec3("dirLight.specular", m_directionalLight.m_specularColor);

    // Point light
    m_shader.setInt("nbPointLights", 1);
    std::string str = "pointLights[0].";
    m_shader.setVec3(str + "position", m_pointLight.m_position);
    m_shader.setVec3(str + "ambient", m_pointLight.m_ambientColor);
    m_shader.setVec3(str + "diffuse", m_pointLight.m_diffuseColor);
    m_shader.setVec3(str + "specular", m_pointLight.m_specularColor);
    m_shader.setFloat(str + "constant", 1.0f);
    m_shader.setFloat(str + "linear", 0.09f);
    m_shader.setFloat(str + "quadratic", 0.032f);

    m_floorMesh->draw(m_shader);
    for(const auto& modelMatrix : m_modelMatrices)
    {
        m_shader.setMat4("model", modelMatrix);
        m_model->draw(m_shader);
    }

    updatePointLightBuffer();
    m_lightCubeShader.bind();
    m_lightCubeShader.setMat4("projectionView", projectionView);
    m_pointLightsVAO.bind();
    glDrawArrays(GL_POINTS, 0, 1);
}

void LightingScene::updatePointLightBuffer()
{
    float pointLightData[6] = {
      m_pointLight.m_position.x,
      m_pointLight.m_position.y,
      m_pointLight.m_position.z,
      m_pointLight.m_diffuseColor.r,
      m_pointLight.m_diffuseColor.g,
      m_pointLight.m_diffuseColor.b,
    };
    m_pointLightsVBO.setData(pointLightData, sizeof(pointLightData));
}

void LightingScene::randomizeModelsPositions()
{
    std::uniform_real_distribution<float> d(0, 10);
    std::uniform_real_distribution<float> angle(0, 360);
    std::normal_distribution<float> n(0.0f, 1.0f);

    m_modelMatrices.clear();

    for(int i = 0; i < 10; ++i)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(d(m_gen) - 5.0f, d(m_gen) / 2.0f, d(m_gen) - 5.0f));
        glm::vec3 axis;
        do
        {
            axis = glm::vec3(n(m_gen), n(m_gen), n(m_gen));
        } while(glm::length2(axis) < 1e-12f);
        axis = glm::normalize(axis);
        modelMatrix = modelMatrix * glm::mat4_cast(glm::quat(glm::angleAxis(angle(m_gen), axis)));

        m_modelMatrices.push_back(modelMatrix);
    }
}

void LightingScene::renderDepthMap()
{
    // For now only handle directional light
    float near{1.0f};
    float far{10.0f};
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
    glm::vec3 directionalLightEstimatedPosition = -m_directionalLight.m_direction * 5.0f;
    glm::mat4 lightView = glm::lookAt(directionalLightEstimatedPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    m_depthShader.bind();
    m_depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, m_shadowsFB.getSpecification().width, m_shadowsFB.getSpecification().height);
    m_shadowsFB.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_depthShader.setMat4("model", glm::mat4(1.0f));
    m_floorMesh->draw(m_depthShader);
    for(const auto& modelMatrix : m_modelMatrices)
    {
        m_depthShader.setMat4("model", modelMatrix);
        m_model->draw(m_depthShader);
    }

    m_shadowsFB.unbind();
}

void LightingScene::displayShadowFrameBuffer()
{
    begin();
    m_depthTexPreviewShader.bind();
    m_quad.bind();
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowsFB.getDepthAttachmentId());
    m_depthTexPreviewShader.setInt("depthMap", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    end();
}