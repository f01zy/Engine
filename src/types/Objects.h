#pragma once

#include <glm.hpp>

struct Object {
  glm::vec3 position;
  int verticesCount;
  int textureTarget;
  int texture;
  int renderFlags;
  unsigned vertexArray;
};
