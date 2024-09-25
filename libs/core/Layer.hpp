#pragma once

#include "CoreExport.hpp"
#include "Events/Event.hpp"
#include "gl.h"

namespace core {

class CORE_API Layer
{
  public:
    virtual void onProcessInput(GLFWwindow* window) = 0;
    virtual void onUpdate() = 0;
    virtual void onEvent(Event& event) = 0;
};

}
