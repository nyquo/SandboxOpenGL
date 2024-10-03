#pragma once

#include "CoreExport.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <string>

namespace core {

namespace fs = std::filesystem;

class CORE_API Shader
{
  public:
    Shader(const fs::path& vertexPath, const fs::path& fragmentPath);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader operator=(const Shader& other) = delete;
    Shader operator=(Shader&& other) = delete;
    ~Shader();

  public:
    void bind() const;
    void unbind() const;

    [[nodiscard]] uint32_t getUid() const { return m_programId; }

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

  private:
    std::string parseFile(const std::filesystem::path& filePath);

  private:
    uint32_t m_programId;
};

}
