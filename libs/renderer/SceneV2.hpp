#pragma once

#include "RendererExport.hpp"
#include "SimulatedEntity.hpp"

#include <memory>
#include <renderer/Camera.hpp>
#include <vector>

class SimulatedEntity;

namespace renderer {

/*
 * future add entity component system
 *
 */
class RENDERER_API SceneV2
{
  public:
    SceneV2();

    auto& getEntites() { return m_entites; }
    const auto& getEntites() const { return m_entites; }

    std::shared_ptr<Camera> getActiveCamera() const { return m_activeCamera; }
    void setActiveCamera(std::shared_ptr<Camera> camera) { m_activeCamera = camera; }

  private:
    std::shared_ptr<Camera> m_activeCamera;
    std::vector<SimulatedEntity> m_entites;
};

}
