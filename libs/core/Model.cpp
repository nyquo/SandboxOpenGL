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
    const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        core::Logger::logError("Error loading model with assimp: " + std::string(importer.GetErrorString()));
        return;
    }

    m_directory = path.parent_path().string();

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    vertices.reserve(mesh->mNumVertices);
    for(size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0])
        {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    indices.reserve(mesh->mNumFaces * 3); // 3 because aiProcess_Triangulate is used
    for(size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTexures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
        loadMaterialTexures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    }

    m_meshes.emplace_back(std::move(vertices), std::move(indices), std::move(textures));
}

void Model::loadMaterialTexures(aiMaterial* mat,
                                aiTextureType type,
                                std::string typeName,
                                std::vector<Texture>& vectorToFill)
{
    for(size_t i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        vectorToFill.emplace_back(str.C_Str(), typeName);
    }
}

}
