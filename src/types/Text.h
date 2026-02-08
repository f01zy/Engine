#pragma once

#include <glm.hpp>

namespace Types {
struct Character {
  unsigned textureId;
  glm::vec2 size;
  glm::vec2 bearing;
  unsigned advance;
};
} // namespace Types
