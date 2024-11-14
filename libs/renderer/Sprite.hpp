#pragma once

#include "Entity.hpp"
#include "RendererExport.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"

#include <Texture.hpp>

#include <vector>

namespace renderer {

class RENDERER_API Sprite : public Entity
{
  public:
    Sprite(const fs::path& path);
    Sprite(const Sprite& other) = delete;
    Sprite(Sprite&& other) noexcept;
    Sprite& operator=(const Sprite& other) = delete;
    Sprite& operator=(Sprite&& other) noexcept;
    ~Sprite();

    void draw(Shader& shader) const final;

    std::string getName() const;

  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int m_vao{0}, m_vbo{0}, m_ebo{0};
    void setupMesh();
    std::string m_textureName;
};

}
