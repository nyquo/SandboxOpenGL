#include "ColoredHouseScene.hpp"

#include <core/gl.h>

ColoredHouseScene::ColoredHouseScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Colored houses", 50, 50, 800, 600, glm::vec3(0.0f))
{
    m_geomTrialShader = std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.vert",
                                                           std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.frag",
                                                           std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.geom");

    // clang-format off
    float points[] = {
      -0.5f, 0.5f, 1.0, 0.0, 0.0, // top-left
      0.5f, 0.5f, 0.0, 1.0, 0.0,// top-right
      0.5f, -0.5f, 0.0, 0.0, 1.0,// bottom-right
      -0.5f, -0.5f, 1.0, 1.0, 0.0// bottom-left
    };
    // clang-format on

    m_vbo.setData(points, sizeof(points));

    renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 2, false, sizeof(float)),
                                  renderer::BufferElement(GL_FLOAT, 3, false, sizeof(float))};
    m_vbo.setLayout(std::move(layout));

    m_vao.addVertexBuffer(m_vbo);
}

void ColoredHouseScene::onEvent(core::Event& event) {}

void ColoredHouseScene::onUpdate()
{
    if(isActive())
    {
        begin();
        drawScene();
        end();
    }
}

void ColoredHouseScene::drawScene()
{
    m_geomTrialShader->bind();
    m_vao.bind();
    glDrawArrays(GL_POINTS, 0, 4);
}