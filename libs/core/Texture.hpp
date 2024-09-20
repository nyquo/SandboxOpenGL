#pragma once

#include "CoreExport.hpp"
#include "gl.h"

#include <stb_image.h>

#include <filesystem>

namespace core {

namespace fs = std::filesystem;

class CORE_API Texture
{
  public:
    Texture(const fs::path& texturePath, int activeTexture);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) = delete;
    Texture operator=(const Texture& other) = delete;
    Texture operator=(Texture&& other) = delete;
    ~Texture();

    void bind() const;

  private:
    GLuint m_textureId;
    int m_activeTexture;
    int m_width, m_height, m_channels;
    unsigned char* m_data;
};

}
