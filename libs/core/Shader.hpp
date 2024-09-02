#pragma once

#include "CoreExport.hpp"

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

    private:
        std::string parseFile(const std::filesystem::path& filePath);

    private:
        uint32_t m_programId;
    };

}