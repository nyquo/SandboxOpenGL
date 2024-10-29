#include "BasicRenderer.hpp"

#include <Logger.hpp>

namespace renderer {

BasicRenderer::BasicRenderer()
{
    m_modelShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag");
    m_outlineShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                               std::string(RESSOURCES_FOLDER) + "/shaders/modelOutline.frag");
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void BasicRenderer::beginFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

void BasicRenderer::renderScene(const Scene& scene, std::shared_ptr<Camera> camera) const
{
    // TEMP draw point lights as cube
    glStencilMask(0x00);
    m_cube.m_shader->bind();
    glBindVertexArray(m_cube.m_vao);
    for(const auto& pointLight : scene.getPointLights())
    {
        m_cube.m_position = pointLight.m_position;

        m_cube.m_modelMatrix = glm::mat4(1.0F);
        m_cube.m_modelMatrix = glm::translate(m_cube.m_modelMatrix, m_cube.m_position);
        m_cube.m_modelMatrix = glm::scale(m_cube.m_modelMatrix, glm::vec3(0.2F));

        m_cube.m_shader->setVec3("lightColor", pointLight.m_diffuseColor);

        m_cube.m_shader->setMat4("view", camera->getView());
        m_cube.m_shader->setMat4("projection", camera->getProjection());
        m_cube.m_shader->setMat4("model", m_cube.m_modelMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // END TEMP

    m_modelShader->bind();

    // Directional light
    const auto& dirLight = scene.getDirectionalLight();
    m_modelShader->setVec3("dirLight.direction", dirLight.m_direction);
    m_modelShader->setVec3("dirLight.ambient", dirLight.m_ambientColor);
    m_modelShader->setVec3("dirLight.diffuse", dirLight.m_diffuseColor);
    m_modelShader->setVec3("dirLight.specular", dirLight.m_specularColor);

    // Point lights
    m_modelShader->setInt("nbPointLights", scene.getPointLights().size());
    int i = 0;
    for(const auto& pointLight : scene.getPointLights())
    {
        std::string str = "pointLights[" + std::to_string(i) + "].";

        m_modelShader->setVec3(str + "position", pointLight.m_position);

        m_modelShader->setVec3(str + "ambient", pointLight.m_ambientColor);
        m_modelShader->setVec3(str + "diffuse", pointLight.m_diffuseColor);
        m_modelShader->setVec3(str + "specular", pointLight.m_specularColor);

        m_modelShader->setFloat(str + "constant", 1.0f);
        m_modelShader->setFloat(str + "linear", 0.09f);
        m_modelShader->setFloat(str + "quadratic", 0.032f);

        ++i;
    }

    // Camera matrices
    m_modelShader->setMat4("view", camera->getView());
    m_modelShader->setMat4("projection", camera->getProjection());
    m_modelShader->setVec3("viewPos", camera->getPosition());

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    for(const auto& model : scene.getModels())
    {
        m_modelShader->setMat4("model", model->getModelMat());
        model->draw(*m_modelShader);
    }

    // Draw Outline
    m_outlineShader->bind();

    m_outlineShader->setVec3("outlineColor", glm::vec3(0.04, 0.28, 0.26));

    // Camera matrices
    m_outlineShader->setMat4("view", camera->getView());
    m_outlineShader->setMat4("projection", camera->getProjection());
    m_outlineShader->setVec3("viewPos", camera->getPosition());

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    for(const auto& model : scene.getModels())
    {
        glm::mat4 modelMatScaled = glm::scale(model->getModelMat(), glm::vec3(1.1f, 1.1f, 1.1f));
        m_outlineShader->setMat4("model", modelMatScaled);
        model->draw(*m_outlineShader);
    }
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::endFrame() {}

void BasicRenderer::setViewport(int width, int height, int x, int y) {}

}
