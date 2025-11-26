#include "ViewportScene.hpp"

#include <core/gl.h>

ViewportScene::ViewportScene(float layerWidth, float layerHeight)
  : Viewport(layerWidth, layerHeight, "Viewport", 50, 50, 800, 600, glm::vec3(0.0f))
{
    m_geomTrialShader =
      std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.vert",
                               std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.frag"/*,
                               std::string(RESSOURCES_FOLDER) + "/shaders/GeomTrial.geom"*/);
}

void ViewportScene::onEvent(core::Event& event) {}

void ViewportScene::onUpdate()
{
    begin();
    // Rendering code would go here
    end();
}