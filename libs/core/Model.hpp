#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <filesystem>
#include <vector>

namespace core {

namespace fs = std::filesystem;

class CORE_API Model
{
  public:
    Model(const fs::path& path);
    Model(const Model& other) = delete;
    Model(Model&& other) = delete;
    Model operator=(const Model& other) = delete;
    Model operator=(Model&& other) = delete;
    ~Model();

    void draw(Shader& shader);

  private:
    void loadModel(const fs::path& path);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTexures(aiMaterial* mat,
                             aiTextureType type,
                             std::string typeName,
                             std::vector<Texture>& vectorToFill);

  private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
};

}
