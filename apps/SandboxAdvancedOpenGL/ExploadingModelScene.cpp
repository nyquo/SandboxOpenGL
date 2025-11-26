#include "ExploadingModelScene.hpp"

#include <core/gl.h>

ExploadingModelScene::ExploadingModelScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Exploading model", 50, 50, 800, 600, glm::vec3(0.0f))
{}

void ExploadingModelScene::onEvent(core::Event& event) {}

void ExploadingModelScene::onUpdate()
{
    if(isActive())
    {
        begin();
        drawScene();
        end();
    }
}

void ExploadingModelScene::drawScene() {}