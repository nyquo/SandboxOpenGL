#include "HappyCube.hpp"

HappyCube::HappyCube(glm::vec3 position) { m_position = position; }

HappyCube::~HappyCube() {}

const float* HappyCube::getRawVertices() const { return m_vertices.data(); }

size_t HappyCube::getVertexCount() const { return m_vertices.size(); }
