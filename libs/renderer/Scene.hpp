#pragma once

#include "RendererExport.hpp"

#include <renderer/Lights.hpp>
#include <renderer/Model.hpp>

#include <vector>

namespace renderer {

class RENDERER_API Scene
{
  public:
    using CModelsIt = std::vector<std::shared_ptr<Model>>::const_iterator;

    Scene();

    /*const*/ auto& getEntities() const { return m_entities; }

    void addEntity(std::shared_ptr<Entity> entity);
    void clearScene();

    void setDirectionalLight(const DirectionalLight& light);
    DirectionalLight getDirectionalLight() const;
    void addPointLight(const PointLight& light);
    void setPointLightVec(const std::vector<PointLight>& lights);
    void clearPointLights();
    const auto& getPointLights() const { return m_pointLights; }

  private:
    std::vector<std::shared_ptr<Entity>> m_entities;

    DirectionalLight m_directionalLight{};
    std::vector<PointLight> m_pointLights{};
};

}
