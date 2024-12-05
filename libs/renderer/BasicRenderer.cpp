#include "BasicRenderer.hpp"

#include <core/Logger.hpp>
#include <Vertex.hpp>
#include <glm/gtx/norm.hpp>

namespace renderer {

BasicRenderer::BasicRenderer()
{
    m_modelShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.vert",
                                             std::string(RESSOURCES_FOLDER) + "/shaders/modelShader.frag");
    m_outlineShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/modelOutline.vert",
                                               std::string(RESSOURCES_FOLDER) + "/shaders/modelOutline.frag");
    m_screenShader = std::make_unique<Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/screenShader.vert",
                                              std::string(RESSOURCES_FOLDER) + "/shaders/screenShader.frag");
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

BasicRenderer::~BasicRenderer()
{
    if(m_offscreen)
    {
        deinitOffscreenRendering();
    }
}

void BasicRenderer::beginFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
    glClearColor(0.008f, 0.082f, 0.149f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

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

void BasicRenderer::setViewport(int width, int height, int x, int y)
{
    m_viewportHeight = height;
    m_viewportWidth = width;
    m_viewportX = x;
    m_viewportY = y;
}

void BasicRenderer::renderSceneOnscreen(const Scene& scene, const std::shared_ptr<Camera> camera)
{
    glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
    renderSceneImpl(scene, camera);
}

void BasicRenderer::renderSceneOffscren(const Scene& scene, const std::shared_ptr<Camera> camera)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_offscreenTextureWidth, m_offscreenTextureHeight);
    renderSceneImpl(scene, camera);
    glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_screenShader->bind();
    m_offscreenQuadVA->bind();
    glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BasicRenderer::renderSceneImpl(const Scene& scene, const std::shared_ptr<Camera> camera)
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

        glBindVertexArray(m_cube.m_vao);
        m_cube.m_indexBuffer.bind();
        glDrawElements(GL_TRIANGLES, m_cube.getIndicesCount(), GL_UNSIGNED_INT, 0);
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

void BasicRenderer::initOffscreenRendering()
{
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glGenTextures(1, &m_textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
    // Todo set proper size corresponding to screen
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, m_offscreenTextureWidth, m_offscreenTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBuffer, 0);

    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_offscreenTextureWidth, m_offscreenTextureHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        core::Logger::logError("Frame buffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<float> verticesAndTexCoords{-1.0f, 1.0f, 0.0f,  1.0f, -1.0f, -1.0f, 0.0f,
                                            0.0f,  1.0f, -1.0f, 1.0f, 0.0f,

                                            -1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  -1.0f, 1.0f,
                                            0.0f,  1.0f, 1.0f,  1.0f, 1.0f

    };
    std::vector<unsigned int> indexes{0, 1, 2, 3, 4, 5};

    m_offscreenQuadVB.reset(new VertexBuffer(verticesAndTexCoords.size() * sizeof(float), verticesAndTexCoords.data()));
    m_offscreenQuadIB.reset(new IndexBuffer(indexes.size() * sizeof(unsigned int), indexes.data()));
    std::vector<VBLayoutElement> layout;
    layout.emplace_back(GL_FLOAT, 2, false);
    layout.emplace_back(GL_FLOAT, 2, false);
    m_offscreenQuadVA.reset(new VertexArray());
    m_offscreenQuadVB->setLayout(std::move(layout));
    m_offscreenQuadVA->setVertexBuffer(m_offscreenQuadVB.get());
    m_offscreenQuadVA->setIndexBuffer(m_offscreenQuadIB.get());
    m_screenShader->bind();
    m_screenShader->setInt("screenTexture", 0);
}

void BasicRenderer::deinitOffscreenRendering()
{
    if(m_frameBuffer != 0)
    {
        glDeleteBuffers(1, &m_frameBuffer);
    }
    if(m_textureColorBuffer != 0)
    {
        glDeleteTextures(1, &m_textureColorBuffer);
    }
    if(m_rbo != 0)
    {
        glDeleteRenderbuffers(1, &m_rbo);
    }
}

}
