#pragma once

#include <glm.hpp>
#include <string>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoordinates;
};

struct Texture {
  unsigned id;
  std::string type;
};
