#include "InstanceRenderingScene.hpp"

#include <core/Logger.hpp>
#include <core/gl.h>
InstanceRenderingScene::InstanceRenderingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Instance Rendering", 50, 50, 800, 600, glm::vec3(0.0f))
  , m_quadShader(std::string(RESSOURCES_FOLDER) + "/shaders/ColoredQuad.vert",
                 std::string(RESSOURCES_FOLDER) + "/shaders/ColoredQuad.frag")
{}

void InstanceRenderingScene::onEvent(core::Event& event) {}

void InstanceRenderingScene::onUpdate()
{
    if(!isActive())
    {
        return;
    }

    // m_camera.setViewPortSize(getWidth(), getHeight());

    begin();
    drawScene();
    end();
}

void InstanceRenderingScene::drawScene() {}