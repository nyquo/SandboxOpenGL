#pragma once

#include "renderer/Buffers.hpp"
#include "renderer/Entity.hpp"
#include "renderer/RendererExport.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Vertex.hpp"

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
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;

    unsigned int m_vao{0};
    std::string m_textureName;
};

}
