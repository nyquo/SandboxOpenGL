#include "ViewportLayer.hpp"

#include <Conversion.hpp>

#include <random>

ViewportLayer::ViewportLayer(float viewportWidth, float viewportHeight)
  : m_viewportWidth(viewportWidth)
  , m_viewportHeight(viewportHeight)
  , m_camera(viewportWidth, viewportHeight, glm::vec3(0.0F, 0.0F, 10.0F))
  , m_lastMouseX(m_viewportWidth / 2)
  , m_lastMouseY(m_viewportHeight / 2)
{
    m_cubeShader = std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.vert",
                                                      std::string(RESSOURCES_FOLDER) + "/shaders/BasicShader.frag");

    m_lightCubeShader =
      std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.vert",
                                         std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.frag");

    // cube VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(m_cube.getRawVertices()) * m_cube.getVertexCount(),
                 m_cube.getRawVertices(),
                 GL_STATIC_DRAW);

    // cube VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light cube VAO
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_cubeTexture =
      std::make_unique<renderer::Texture>(std::string(RESSOURCES_FOLDER) + "/assets/container2.png", "texture_diffuse");
    glActiveTexture(GL_TEXTURE0);
    m_cubeSpecularTexture = std::make_unique<renderer::Texture>(
      std::string(RESSOURCES_FOLDER) + "/assets/container2_specular.png", "texture_specular");
    glActiveTexture(GL_TEXTURE1);

    // TEMP
    for(int i = 0; i < 16; ++i)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 80);

        glm::vec3 pos{((float)dist(rng) - 40) / 10, ((float)dist(rng) - 40) / 10, ((float)dist(rng) - 40) / 10};

        dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 100);
        glm::vec3 rotVector{(((float)dist(rng)) / 100, ((float)dist(rng)) / 100, ((float)dist(rng)) / 100)};

        dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 360);
        int rotationDegree = dist(rng);

        glm::mat4 modelMat{1.0F};
        modelMat = glm::translate(modelMat, pos);
        modelMat = glm::rotate(modelMat, (float)rotationDegree, rotVector);
        m_cubeModelMatrix.push_back(modelMat);
    }

    glBindVertexArray(0);
}

ViewportLayer::~ViewportLayer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ViewportLayer::onUpdate()
{
    processInputs();

    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // DRAW LIGHTS
    m_lightCubeShader->bind();

    glBindVertexArray(m_lightVAO);

    for(auto& lightCubePos : m_guiData.m_lightCubePos)
    {
        m_lightCube.m_position = lightCubePos;

        m_lightCube.m_modelMatrix = glm::mat4(1.0F);
        m_lightCube.m_modelMatrix = glm::translate(m_lightCube.m_modelMatrix, m_lightCube.m_position);
        m_lightCube.m_modelMatrix = glm::scale(m_lightCube.m_modelMatrix, glm::vec3(0.2F));

        m_lightCubeShader->setVec3("lightColor", utils::toGlmVec4(m_guiData.m_diffusePointLight));

        m_lightCubeShader->setMat4("view", m_camera.getView());
        m_lightCubeShader->setMat4("projection", m_camera.getProjection());
        m_lightCubeShader->setMat4("model", m_lightCube.m_modelMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // DRAW CUBES
    m_cubeShader->bind();

    // directional light
    m_cubeShader->setVec3("dirLight.direction", m_guiData.m_dirLightDirection);
    m_cubeShader->setVec3("dirLight.ambient", utils::toGlmVec4(m_guiData.m_ambientDirLight));
    m_cubeShader->setVec3("dirLight.diffuse", utils::toGlmVec4(m_guiData.m_diffuseDirLight));
    m_cubeShader->setVec3("dirLight.specular", utils::toGlmVec4(m_guiData.m_specularDirLight));

    // point lights
    for(int i = 0; i < m_guiData.m_lightCubePos.size(); ++i)
    {
        std::string str = "pointLights[" + std::to_string(i) + "].";

        m_cubeShader->setVec3(str + "position", m_guiData.m_lightCubePos[i]);

        m_cubeShader->setVec3(str + "ambient", utils::toGlmVec4(m_guiData.m_ambientPointLight));
        m_cubeShader->setVec3(str + "diffuse", utils::toGlmVec4(m_guiData.m_specularPointLight));
        m_cubeShader->setVec3(str + "specular", utils::toGlmVec4(m_guiData.m_specularPointLight));

        m_cubeShader->setFloat(str + "constant", 1.0f);
        m_cubeShader->setFloat(str + "linear", 0.09f);
        m_cubeShader->setFloat(str + "quadratic", 0.032f);
    }

    // spot light (flash light)
    m_cubeShader->setVec3("spotLight.position", m_camera.getPosition());
    m_cubeShader->setVec3("spotLight.direction", m_camera.getDirection());
    m_cubeShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(m_guiData.m_cutOff)));
    m_cubeShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(m_guiData.m_outerCutOff)));

    m_cubeShader->setVec3("spotLight.ambient", utils::toGlmVec4(m_guiData.m_ambientFlashLight));
    m_cubeShader->setVec3("spotLight.diffuse", utils::toGlmVec4(m_guiData.m_diffuseFlashLight));
    m_cubeShader->setVec3("spotLight.specular", utils::toGlmVec4(m_guiData.m_specularFlashLight));

    m_cubeShader->setFloat("spotLight.constant", 1.0f);
    m_cubeShader->setFloat("spotLight.linear", 0.09f);
    m_cubeShader->setFloat("spotLight.quadratic", 0.032f);

    // material properties
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cubeTexture->getId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_cubeSpecularTexture->getId());
    m_cubeShader->setInt("material.diffuse", 0);
    m_cubeShader->setInt("material.specular", 1);
    m_cubeShader->setFloat("material.shininess", (float)m_guiData.m_shininess); // 2 ->512?

    // transform matrices
    m_cubeShader->setMat4("view", m_camera.getView());
    m_cubeShader->setMat4("projection", m_camera.getProjection());
    m_cubeShader->setVec3("viewPos", m_camera.getPosition());

    glBindVertexArray(VAO);

    for(auto& cube : m_cubeModelMatrix)
    {
        m_cubeShader->setMat4("model", cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void ViewportLayer::onEvent(core::Event& e)
{
    core::EventDispatcher dispatcher(e);
    dispatcher.dispatch<core::MouseScrolledEvent>(BIND_EVENT_FN(ViewportLayer::onMouseScrolled));
    dispatcher.dispatch<core::MouseMovedEvent>(BIND_EVENT_FN(ViewportLayer::onMouseMoved));
}

void ViewportLayer::setViewportSize(float viewportWidth, float viewportHeight)
{
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_camera.setViewPortSize(viewportWidth, viewportHeight);
}

void ViewportLayer::setCameraMovement(bool cameraMovementEnabled) { m_cameraMovementEnabled = cameraMovementEnabled; }

void ViewportLayer::setGuiData(const GuiData& guiData) { m_guiData = guiData; }

void ViewportLayer::processInputs()
{
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    if(m_cameraMovementEnabled)
    {
        const float cameraSpeed = 10.0F * m_deltaTime;
        glm::vec3 positionOffset{0.0F, 0.0F, 0.0F};

        if(isKeyPressed(GLFW_KEY_W))
        {
            positionOffset.z += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_S))
        {
            positionOffset.z -= cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_A))
        {
            positionOffset.x -= cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_D))
        {
            positionOffset.x += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_SPACE))
        {
            positionOffset.y += cameraSpeed;
        }
        if(isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        {
            positionOffset.y -= cameraSpeed;
        }
        m_camera.translateCameraRelative(positionOffset);
    }
}

bool ViewportLayer::onMouseScrolled(core::MouseScrolledEvent& e)
{
    if(m_cameraMovementEnabled)
    {
        m_camera.adustFov(-m_zoomOffset * e.getYOffset());
    }
    return true;
}

bool ViewportLayer::onMouseMoved(core::MouseMovedEvent& e)
{
    if(m_firstMouse)
    {
        m_lastMouseX = e.getX();
        m_lastMouseY = e.getY();
        m_firstMouse = false;
    }
    float xOffset = e.getX() - m_lastMouseX;
    float yOffset = m_lastMouseY - e.getY();
    m_lastMouseX = e.getX();
    m_lastMouseY = e.getY();
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    if(m_cameraMovementEnabled)
    {
        m_camera.rotateCamera(xOffset, yOffset);
    }

    return true;
}
