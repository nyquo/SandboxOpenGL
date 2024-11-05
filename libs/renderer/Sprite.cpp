#include "Sprite.hpp"

#include <gl.h>

namespace renderer {

Sprite::Sprite(const fs::path& path)
{
    Vertex tl{{-0.5, 0.5, 0.0}, {0, 0, 1}, {0, 1}};
    Vertex tr{{0.5, 0.5, 0.0}, {0, 0, 1}, {1, 1}};
    Vertex bl{{-0.5, -0.5, 0.0}, {0, 0, 1}, {0, 0}};
    Vertex br{{0.5, -0.5, 0.0}, {0, 0, 1}, {1, 0}};

    vertices.push_back(tl);
    vertices.push_back(tr);
    vertices.push_back(bl);
    vertices.push_back(br);

    indices.insert(indices.end(), {0, 1, 2, 1, 3, 2});

    textures.emplace_back(path.string().c_str(), "texture_diffuse1");

    setupMesh();

    m_textureName = path.filename().stem().string();
};

Sprite::Sprite(Sprite&& other) noexcept
  : vertices(std::move(other.vertices))
  , indices(std::move(other.indices))
  , textures(std::move(other.textures))
  , m_vao(other.m_vao)
  , m_vbo(other.m_vbo)
  , m_ebo(other.m_ebo)
{
    other.m_ebo = 0;
    other.m_vbo = 0;
    other.m_vao = 0;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept
{
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);
    m_ebo = other.m_ebo;
    m_vbo = other.m_vbo;
    m_vao = other.m_vao;
    other.m_ebo = 0;
    other.m_vbo = 0;
    other.m_vao = 0;

    return *this;
}

Sprite::~Sprite()
{
    if(m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
    }
    if(m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
    }
    if(m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
    }
}

void Sprite::draw(Shader& shader) const
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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::string Sprite::getName() const { return m_textureName; }

void Sprite::setupMesh()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glBindVertexArray(0);
}
}
