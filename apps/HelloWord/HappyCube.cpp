#include "HappyCube.hpp"

HappyCube::HappyCube() {}

HappyCube::~HappyCube() {}

const float* HappyCube::getRawVertices() const { return m_vertices.data(); }

size_t HappyCube::getVertexCount() const { return m_vertices.size(); }

glm::mat4 HappyCube::getModelMatrix() const { return m_modelMatrix; }

void HappyCube::setModelMatrix(glm::mat4 modelMatrix) { m_modelMatrix = modelMatrix; }
