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
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept
  : vertices(std::move(other.vertices))
  , indices(std::move(other.indices))
  , textures(std::move(other.textures))
  , m_vertexBuffer(std::move(other.m_vertexBuffer))
  , m_indexBuffer(std::move(other.m_indexBuffer))
  , m_vao(other.m_vao)
{
    other.m_vao = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_indexBuffer = std::move(other.m_indexBuffer);
    m_vao = other.m_vao;
    other.m_vao = 0;

    return *this;
}

Mesh::~Mesh()
{
    if(m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
    }
}

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

    glBindVertexArray(m_vao);
    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}
