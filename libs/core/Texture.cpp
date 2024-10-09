#include "Texture.hpp"

#include "Logger.hpp"

namespace core {

// TODO Add static dic to load text only once

Texture::Texture(const fs::path& texturePath)
{
    static bool isInit{false};
    if(!isInit)
    {
        stbi_set_flip_vertically_on_load(true);
    }

    m_data = stbi_load(texturePath.string().c_str(), &m_width, &m_height, &m_channels, 0);

    if(!m_data)
    {
        core::Logger::logError("Could not load texture: " + texturePath.string());
    }

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    GLenum format;

    if(m_channels == 1)
    {
        format = GL_RED;
    }
    else if(m_channels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        format = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(m_data);
}

Texture::Texture(Texture&& other) noexcept
  : m_textureId(other.m_textureId)
  , m_width(other.m_width)
  , m_height(other.m_height)
  , m_channels(other.m_channels)
  , m_data(std::move(other.m_data))
  , m_type(std::move(other.m_type))
{
    // Prevent the texture from being deleted (using glDeleteTexture)
    other.m_textureId = 0;
}

Texture::~Texture()
{
    if(m_textureId != 0)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

std::string Texture::getType() const { return m_type; }

unsigned int Texture::getId() const { return m_textureId; }

}
