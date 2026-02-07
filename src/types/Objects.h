#pragma once

#include <glm.hpp>

struct Object {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
  float angle = 0.0f;
  unsigned textureTarget = 0;
  unsigned texture = 0;
  unsigned vertexArray;
  int verticesCount;
  int renderFlags;
};

const int DEPTH_TEST = 1 << 0;
const int DEPTH_LEQUAL = 1 << 1;
const int CULL_FACE = 1 << 2;
const int SEPARATE_MATRICES = 1 << 3;
const int NO_TRANSFORM = 1 << 4;
const int RENDER_IN_THE_END = 1 << 5;
const int SCALE = 1 << 6;
const int ROTATE = 1 << 7;
