#pragma once

#include <unordered_map>
#include <vector>

#include "../ResourceManager/ResourceManager.h"
#include "../types/Objects.h"

class Objects final {
public:
  Objects();
  Objects(Objects &) = delete;
  void addObject(const Types::Object &object, const unsigned shaderId);
  void render(ResourceManager &resourceManager, unsigned matricesUBO, glm::mat4 &view, glm::mat4 &projection);

private:
  std::unordered_map<unsigned, std::vector<Types::Object>> objects;
};
