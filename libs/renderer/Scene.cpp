#include "Scene.hpp"

namespace renderer {

Scene::Scene()
{
    // Arbitrary say that a scene will probably contain more than 20 objects
    m_models.reserve(20);
    // Arbitrary say that their will be no more than 10 point lights
    m_pointLights.reserve(10);
}

void Scene::addModel(std::shared_ptr<Model> model) { m_models.push_back(model); }

void Scene::clearScene() { m_models.clear(); }

void Scene::setDirectionalLight(const DirectionalLight& light) { m_directionalLight = light; }

DirectionalLight Scene::getDirectionalLight() const { return m_directionalLight; }

void Scene::addPointLight(const PointLight& light) { m_pointLights.push_back(light); }

void Scene::setPointLightVec(const std::vector<PointLight>& lights) { m_pointLights = lights; }

void Scene::clearPointLights() { m_pointLights.clear(); }

}
