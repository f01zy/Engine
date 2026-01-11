#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"

class Model final {
public:
  Model(std::string path) { loadModel(path); };
  Model(Model &) = delete;
  void draw(Shader &shader);

private:
  std::vector<Types::Texture> texturesLoaded;
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Types::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
