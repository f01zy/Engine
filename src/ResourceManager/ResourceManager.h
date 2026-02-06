#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Shader/Shader.h"

class ResourceManager final {
public:
  ResourceManager();
  ResourceManager(ResourceManager &) = delete;
  unsigned loadShader(std::string vertexPath, std::string fragmentPath);
  Shader &getShaderById(unsigned shaderId);

private:
  std::unordered_map<unsigned, std::unique_ptr<Shader>> shaders;
};
