#pragma once

#include <string>
#include <unordered_map>

#include "../Shader/Shader.h"
#include "../types/Text.h"

class Text final {
public:
  Text(std::string font);
  Text(Text &) = delete;
  void render(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4 &projection);

private:
  std::unordered_map<char, Types::Character> characters;
  unsigned textVAO, textVBO;
};
