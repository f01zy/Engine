#pragma once

#include <vector>

#include "../Shader/Shader.h"
#include "../types/Mesh.h"

class Mesh final {
public:
  Mesh(std::vector<Types::Vertex> &vertices, std::vector<Types::Texture> &textures, std::vector<unsigned> &indices);
  void draw(Shader &shader);

private:
  std::vector<Types::Vertex> vertices;
  std::vector<Types::Texture> textures;
  std::vector<unsigned> indices;
  unsigned VAO, VBO, IBO;

  void setupMesh();
};
