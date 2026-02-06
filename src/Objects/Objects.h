#pragma once

#include <unordered_map>
#include <vector>

#include "../ResourceManager/ResourceManager.h"
#include "../types/Objects.h"

class Objects final {
public:
  Objects();
  Objects(Objects &) = delete;
  void addObject(const Object &object, const unsigned shaderId);
  void draw(ResourceManager &resourceManager);

private:
  std::unordered_map<unsigned, std::vector<Object>> objects;
};
