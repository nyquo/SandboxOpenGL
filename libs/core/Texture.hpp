#pragma once

#include "CoreExport.hpp"
#include "gl.h"

#include <stb_image.h>

#include <filesystem>
#include <string>

namespace core {

namespace fs = std::filesystem;

class CORE_API Texture
{
  public:
    Texture(const fs::path& texturePath, std::string type);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();

    [[nodiscard]] std::string getType() const;
    [[nodiscard]] unsigned int getId() const;

  private:
    GLuint m_textureId;
    int m_width, m_height, m_channels;
    unsigned char* m_data;
    std::string m_type;
};

}
