#pragma once

#include <glm.hpp>

struct Character {
  unsigned textureId;
  glm::vec2 size;
  glm::vec2 bearing;
  unsigned advance;
};
