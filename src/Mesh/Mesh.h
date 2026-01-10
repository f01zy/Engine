#pragma once

#include <vector>

#include "../Shader/Shader.h"
#include "../types/Mesh.h"

class Mesh final {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned> indices);
  void draw(Shader &shader);

private:
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned> indices;
  unsigned VAO, VBO, IBO;

  void setupMesh();
};
