#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

namespace utils {

glm::vec4 toGlmVec4(ImVec4 vec) { return glm::vec4{vec.x, vec.y, vec.z, vec.w}; }

}
