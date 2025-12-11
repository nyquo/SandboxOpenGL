#pragma once

#include "RendererExport.hpp"

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace renderer {

namespace fs = std::filesystem;

class RENDERER_API Shader
{
  public:
    Shader(const fs::path& vertexPath, const fs::path& fragmentPath, const fs::path& geometryPath = {});
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader operator=(const Shader& other) = delete;
    Shader operator=(Shader&& other) = delete;
    ~Shader();

  public:
    void bind() const;
    void unbind() const;

    [[nodiscard]] uint32_t getUid() const { return m_programId; }

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;

  private:
    std::string parseFile(const std::filesystem::path& filePath);

  private:
    uint32_t m_programId;
};

}
