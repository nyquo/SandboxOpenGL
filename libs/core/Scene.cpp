#include "Scene.hpp"

namespace core {

Scene::Scene()
{
    // Arbitrary say that a scene will probably contain more than 20 objects
    m_models.reserve(20);
}

Scene::CModelsIt Scene::cbegin() const { return m_models.cbegin(); }

Scene::CModelsIt Scene::cend() const { return m_models.cend(); }

void Scene::addModel(std::shared_ptr<Model> model) { m_models.push_back(model); }

void Scene::clearScene() { m_models.clear(); }

}
