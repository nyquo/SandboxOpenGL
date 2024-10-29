#pragma once

#include "Mesh.hpp"
#include "RendererExport.hpp"
#include "Shader.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <filesystem>
#include <vector>

namespace renderer {

namespace fs = std::filesystem;

class RENDERER_API Model
{
  public:
    Model(const fs::path& path);
    Model(const Model& other) = delete;
    Model(Model&& other) = delete;
    Model operator=(const Model& other) = delete;
    Model operator=(Model&& other) = delete;
    ~Model();

    void draw(Shader& shader) const;
    glm::mat4 getModelMat() const;
    void setModelMat(const glm::mat4& modelMat);
    std::string getName() const;

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
    glm::mat4 m_modelMat{1.0f};
    std::string m_directory{""};
    std::string m_modelName{""};

    // temp
  public:
    bool outline{false};
};

}
