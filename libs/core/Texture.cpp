#include "Texture.hpp"

#include "Logger.hpp"

namespace core {

// TODO integrate 3D texture and see if that belongs in an other lib

Texture::Texture(const fs::path& texturePath, int activeTexture)
  : m_activeTexture(activeTexture)
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
    glActiveTexture(m_activeTexture);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;

    if(m_channels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        format = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(m_data);
}

Texture::~Texture() {}

void Texture::bind() const
{
    glActiveTexture(m_activeTexture);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

}
