#pragma once

#include "RendererExport.hpp"

#include <core/gl.h>
#include <io/stb_image.h>

#include <filesystem>
#include <map>
#include <string>

namespace renderer {

namespace fs = std::filesystem;

class RENDERER_API Texture
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
    [[nodiscard]] GLenum getFormat() const;

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
    GLenum m_format{GL_RGB};

    static std::map<fs::path, std::pair<unsigned int, TextureData>> m_loadedCount;
};

}
