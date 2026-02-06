#pragma once

#include <unordered_map>
#include <vector>

#include "../Shader/Shader.h"
#include "../types/Objects.h"

class Objects final {
public:
  Objects();
  Objects(Objects &) = delete;
  void addObject(Object &object, unsigned shaderId);
  void draw(std::unordered_map<unsigned, Shader &> shaders);

private:
  std::unordered_map<int, std::vector<Object>> objects;
};
