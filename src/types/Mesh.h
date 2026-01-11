#pragma once

#include <glm.hpp>
#include <string>

namespace Types {
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoordinates;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct Texture {
  unsigned id;
  std::string type;
  std::string path;
};
} // namespace Types
