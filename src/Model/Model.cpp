#include <iostream>

#include "../Texture/Texture.h"
#include "Model.h"

void Model::draw(Shader &shader) {
  for (int i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader);
  }
}

void Model::loadModel(std::string path) {
  std::cout << "Loading model: " << path << "\n";
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  for (int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Types::Vertex> vertices;
  std::vector<Types::Texture> textures;
  std::vector<unsigned> indices;

  for (int i = 0; i < mesh->mNumVertices; i++) {
    Types::Vertex vertex;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    position.x = mesh->mVertices[i].x;
    position.y = mesh->mVertices[i].y;
    position.z = mesh->mVertices[i].z;
    vertex.position = position;

    normal.x = mesh->mNormals[i].x;
    normal.y = mesh->mNormals[i].y;
    normal.z = mesh->mNormals[i].z;
    vertex.normal = normal;

    if (mesh->HasTextureCoords(0)) {
      glm::vec2 textureCoordinates;
      textureCoordinates.x = mesh->mTextureCoords[0][i].x;
      textureCoordinates.y = mesh->mTextureCoords[0][i].y;
      vertex.textureCoordinates = textureCoordinates;
    } else {
      vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
    }

    tangent.x = mesh->mTangents[i].x;
    tangent.y = mesh->mTangents[i].y;
    tangent.z = mesh->mTangents[i].z;
    vertex.tangent = tangent;

    bitangent.x = mesh->mBitangents[i].x;
    bitangent.y = mesh->mBitangents[i].y;
    bitangent.z = mesh->mBitangents[i].z;
    vertex.bitangent = bitangent;
    vertices.push_back(vertex);
  }

  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Types::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "textureDiffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Types::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "textureSpecular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<Types::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "textureNormal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<Types::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "textureHeight");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  }

  return Mesh(vertices, textures, indices);
}

std::vector<Types::Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
  std::vector<Types::Texture> textures;
  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (int j = 0; j < texturesLoaded.size(); j++) {
      if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(texturesLoaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Types::Texture texture;
      std::string texturePath = directory + "/" + str.C_Str();
      texture.id = Texture(texturePath).getTexture();
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      texturesLoaded.push_back(texture);
    }
  }
  return textures;
}
