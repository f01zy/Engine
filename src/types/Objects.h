#pragma once

#include <glm.hpp>

struct Object {
  glm::vec3 position;
  glm::vec3 rotate;
  glm::vec3 scale;
  int verticesCount;
  int textureTarget;
  int texture;
  int renderFlags;
  unsigned vertexArray;
};

const int DEPTH_TEST = 1 << 0;
const int DEPTH_LEQUAL = 1 << 1;
const int CULL_FACE = 1 << 2;
const int SEPARATE_MATRICES = 1 << 3;
const int NO_TRANSFORM = 1 << 4;
const int RENDER_IN_THE_END = 1 << 5;
