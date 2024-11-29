#include "Mesh.hpp"

#include <gl.h>

namespace renderer {

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures)
  : vertices(std::move(vertices))
  , indices(std::move(indices))
  , textures(std::move(textures))
  , m_vertexBuffer(this->vertices.size() * sizeof(Vertex), this->vertices.data())
  , m_indexBuffer(this->indices.size() * sizeof(unsigned int), this->indices.data())
{
    std::vector<VBLayoutElement> layout;
    layout.emplace_back(GL_FLOAT, 3, false);
    layout.emplace_back(GL_FLOAT, 3, false);
    layout.emplace_back(GL_FLOAT, 2, false);
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.setVertexBuffer(&m_vertexBuffer);
    m_vertexArray.setIndexBuffer(&m_indexBuffer);
}

Mesh::Mesh(Mesh&& other) noexcept
  : vertices(std::move(other.vertices))
  , indices(std::move(other.indices))
  , textures(std::move(other.textures))
  , m_vertexBuffer(std::move(other.m_vertexBuffer))
  , m_indexBuffer(std::move(other.m_indexBuffer))
  , m_vertexArray(std::move(other.m_vertexArray))
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_indexBuffer = std::move(other.m_indexBuffer);
    m_vertexArray = std::move(other.m_vertexArray);

    return *this;
}

Mesh::~Mesh() {}

void Mesh::draw(Shader& shader) const
{
    unsigned int diffuseNb = 1;
    unsigned int specularNb = 1;
    for(unsigned int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].getType();
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNb++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNb++);
        }
        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].getId());
    }
    shader.setFloat("material.shininess", 16);
    glActiveTexture(GL_TEXTURE0);

    m_vertexArray.bind();
    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}
