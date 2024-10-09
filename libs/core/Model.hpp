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

class Model
{
  public:
    Model(fs::path& path);
    Model(const Model& other) = delete;
    Model(Model&& other) = delete;
    Model operator=(const Model& other) = delete;
    Model operator=(Model&& other) = delete;
    ~Model();

    void draw(Shader& shader);

  private:
    void loadModel(fs::path& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTexures(aiMaterial* mat, aiTextureType type, std::string typeName);

  private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
};

}
