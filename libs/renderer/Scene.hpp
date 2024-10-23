#pragma once

#include "RendererExport.hpp"

#include <Lights.hpp>
#include <Model.hpp>

#include <vector>

namespace renderer {

// TODO optimize all the copies made here and between imgui layers
class RENDERER_API Scene
{
  public:
    using CModelsIt = std::vector<std::shared_ptr<Model>>::const_iterator;

    Scene();

    const auto& getModels() const { return m_models; }

    void addModel(std::shared_ptr<Model> model);
    void clearScene();

    void setDirectionalLight(const DirectionalLight& light);
    DirectionalLight getDirectionalLight() const;
    void addPointLight(const PointLight& light);
    void setPointLightVec(const std::vector<PointLight>& lights);
    void clearPointLights();
    const auto& getPointLights() const { return m_pointLights; }

  private:
    std::vector<std::shared_ptr<Model>> m_models;

    DirectionalLight m_directionalLight{};
    std::vector<PointLight> m_pointLights{};
};

}
