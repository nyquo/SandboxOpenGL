#include "Texture.hpp"

#include "core/Logger.hpp"

namespace renderer {

std::map<fs::path, std::pair<unsigned int, Texture::TextureData>> Texture::m_loadedCount = {};

Texture::Texture(const fs::path& texturePath, std::string type)
  : m_path(texturePath)
{
    static bool isInit{false};
    if(!isInit)
    {
        stbi_set_flip_vertically_on_load(true);
    }

    if(auto it = m_loadedCount.find(texturePath); it != m_loadedCount.end())
    {
        // Already loaded
        m_textureData = it->second.second;
        it->second.first++; // augment count;
    }
    else
    {
        // Not already loaded
        m_textureData.type = type;
        m_textureData.data = stbi_load(
          texturePath.string().c_str(), &m_textureData.width, &m_textureData.height, &m_textureData.channels, 0);

        if(!m_textureData.data)
        {
            core::Logger::logError("Could not load texture: " + texturePath.string());
        }

        glGenTextures(1, &m_textureData.textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureData.textureId);

        if(m_textureData.channels == 1)
        {
            m_format = GL_RED;
        }
        else if(m_textureData.channels == 4)
        {
            m_format = GL_RGBA;
        }
        else
        {
            m_format = GL_RGB;
        }

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     m_format,
                     m_textureData.width,
                     m_textureData.height,
                     0,
                     m_format,
                     GL_UNSIGNED_BYTE,
                     m_textureData.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if(m_format == GL_RGBA)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(m_textureData.data);

        m_loadedCount[texturePath] = std::make_pair(1, m_textureData);
    }
}

Texture::Texture(Texture&& other) noexcept
  : m_textureData(std::move(other.m_textureData))
  , m_path(std::move(other.m_path))
{
    // Prevent the texture from being deleted (using glDeleteTexture)
    other.m_textureData.textureId = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_textureData = std::move(other.m_textureData);
    m_path = std::move(other.m_path);

    // Prevent the texture from being deleted (using glDeleteTexture)
    other.m_textureData.textureId = 0;

    return *this;
}

Texture::~Texture()
{
    // m_data is already deleted in the constructor by stbi_image_free
    if(m_textureData.textureId != 0)
    {
        m_loadedCount[m_path].first--;
        if(m_loadedCount[m_path].first == 0)
        {
            m_loadedCount.erase(m_path);
            glDeleteTextures(1, &m_textureData.textureId);
        }
    }
}

std::string Texture::getType() const { return m_textureData.type; }

unsigned int Texture::getId() const { return m_textureData.textureId; }

GLenum Texture::getFormat() const { return m_format; }

}
