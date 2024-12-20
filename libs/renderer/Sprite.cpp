#include "Sprite.hpp"

#include <core/gl.h>

namespace renderer {

Sprite::Sprite(const fs::path& path)
  : m_vertices({{{-0.5, 0.5, 0.0}, {0, 0, 1}, {0, 1}},
                {{0.5, 0.5, 0.0}, {0, 0, 1}, {1, 1}},
                {{-0.5, -0.5, 0.0}, {0, 0, 1}, {0, 0}},
                {{0.5, -0.5, 0.0}, {0, 0, 1}, {1, 0}}})
  , m_indices({0, 1, 2, 1, 3, 2})
  , m_textures()
  , m_vertexBuffer(m_vertices.size() * sizeof(Vertex), m_vertices.data())
  , m_indexBuffer(m_indices.size(), m_indices.data())
{
    m_textures.emplace_back(path.string().c_str(), "texture_diffuse1");
    // If texture could be loaded
    if(!m_textures.empty())
    {
        if(m_textures[0].getFormat() == GL_RGBA)
        {
            m_hasTransparentColor = true;
        }
    }
    m_textures.emplace_back(fs::path(RESSOURCES_FOLDER) / "assets" / "DarkGrey.png", "texture_specular1");

    BufferLayout layout{BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                        BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                        BufferElement(GL_FLOAT, 2, false, sizeof(float))};
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);

    m_textureName = path.filename().stem().string();
};

Sprite::Sprite(Sprite&& other) noexcept
  : m_vertices(std::move(other.m_vertices))
  , m_indices(std::move(other.m_indices))
  , m_textures(std::move(other.m_textures))
  , m_vertexBuffer(std::move(other.m_vertexBuffer))
  , m_indexBuffer(std::move(other.m_indexBuffer))
  , m_vertexArray(std::move(other.m_vertexArray))

{
}

Sprite& Sprite::operator=(Sprite&& other) noexcept
{
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_textures = std::move(other.m_textures);
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_indexBuffer = std::move(other.m_indexBuffer);
    m_vertexArray = std::move(other.m_vertexArray);

    return *this;
}

Sprite::~Sprite()
{
}

void Sprite::draw(Shader& shader) const
{
    unsigned int diffuseNb = 1;
    unsigned int specularNb = 1;
    for(unsigned int i = 0; i < m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].getType();
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNb++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNb++);
        }
        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].getId());
    }
    shader.setFloat("material.shininess", 16);
    glActiveTexture(GL_TEXTURE0);

    m_vertexArray.bind();
    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::string Sprite::getName() const { return m_textureName; }

}
