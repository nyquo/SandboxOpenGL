#include "LightingScene.hpp"

#include <core/gl.h>

LightingScene::LightingScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Lighting Scene", 50, 50, 800, 600, glm::vec3(0.0f))
{}

void LightingScene::onEvent(core::Event& event) {}

void LightingScene::onUpdate()
{
    if(isActive())
    {
        return;
    }
    begin();
    drawScene();
    end();
}

void LightingScene::drawScene() {}