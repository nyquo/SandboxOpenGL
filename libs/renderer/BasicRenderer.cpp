#include "BasicRenderer.hpp"

#include <Logger.hpp>
#include <glm/gtx/norm.hpp>

namespace renderer {

BasicRenderer::BasicRenderer()
{
    m_modelShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag");
    m_outlineShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelOutline.vert",
                                               std::string(RESSOURCES_FOLDER) + "/shaders/modelOutline.frag");
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void BasicRenderer::beginFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

void BasicRenderer::renderScene(const Scene& scene, std::shared_ptr<Camera> camera)
{
    if(m_offscreen)
    {
        if(m_frameBuffer == 0)
        {
            initOffscreenRendering(); // move this to the function that will enable/disable offscreen
        }
        renderSceneOffscren(scene, camera);
    }
    else
    {
        if(m_frameBuffer != 0)
        {
            deinitOffscreenRendering();
        }
        renderSceneOnscreen(scene, camera);
    }
}

void BasicRenderer::endFrame() {}

void BasicRenderer::setViewport(int width, int height, int x, int y) { glViewport(x, y, width, height); }

void BasicRenderer::renderSceneOnscreen(const Scene& scene, const std::shared_ptr<Camera> camera)
{
    if(m_wireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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
    std::map<float, std::shared_ptr<renderer::Entity>> transparentSorted;
    for(const auto& model : scene.getEntities())
    {
        if(model->outline)
        {
            glStencilMask(0xFF);
        }
        else
        {
            glStencilMask(0x00);
        }
        if(!model->hasTransparentColor())
        {
            m_modelShader->setMat4("model", model->getModelMat());
            model->draw(*m_modelShader);
        }
        else
        {
            float distance = glm::length2(camera->getPosition() - glm::vec3(model->getModelMat()[0]));
            transparentSorted[distance] = model;
        }
    }

    for(auto it = transparentSorted.rbegin(); it != transparentSorted.rend(); ++it)
    {
        m_modelShader->setMat4("model", it->second->getModelMat());
        it->second->draw(*m_modelShader);
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
    for(const auto& model : scene.getEntities())
    {
        if(model->outline)
        {
            m_outlineShader->setMat4("model", model->getModelMat());
            model->draw(*m_outlineShader);
        }
    }
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::renderSceneOffscren(const Scene& scene, const std::shared_ptr<Camera> camera) {}

void BasicRenderer::initOffscreenRendering() {}

void BasicRenderer::deinitOffscreenRendering() {}

}
