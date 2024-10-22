#pragma once

#include "CoreExport.hpp"

#include <Model.hpp>

#include <vector>

namespace core {

class CORE_API Scene
{
  public:
    using CModelsIt = std::vector<std::shared_ptr<Model>>::const_iterator;

    Scene();

    CModelsIt cbegin() const;
    CModelsIt cend() const;

    void addModel(std::shared_ptr<Model> model);

    void clearScene();

  private:
    std::vector<std::shared_ptr<Model>> m_models;
};

}
