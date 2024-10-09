#include "Model.hpp"

#include <Logger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace core {

Model::Model(fs::path& path) {}

Model::~Model() {}

void Model::draw(Shader& shader)
{
    for(auto& mesh : m_meshes)
    {
        mesh.draw(shader);
    }
}

void Model::loadModel(fs::path& path)
{
    Assimp::Importer importer;
    // TODO consider using aiProcess_GenNormals
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        core::Logger::logError("Error loading model with assimp: " + std::string(importer.GetErrorString()));
        return;
    }

    m_directory = path.parent_path();

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {}

std::vector<Texture> Model::loadMaterialTexures(aiMaterial* mat, aiTextureType type, std::string typeName) {}

}
