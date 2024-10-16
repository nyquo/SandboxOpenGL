#pragma once

#include "CoreExport.hpp"
#include "gl.h"

#include <stb_image.h>

#include <filesystem>
#include <map>
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
    struct TextureData
    {
        GLuint textureId;
        int width, height, channels;
        unsigned char* data;
        std::string type;
    };

    TextureData m_textureData;
    fs::path m_path;

    static std::map<fs::path, std::pair<unsigned int, TextureData>> m_loadedCount;
};

}
