#include "ResourceManager.h"
#include <memory>

ResourceManager::ResourceManager() {}

unsigned ResourceManager::loadShader(std::string vertexPath, std::string fragmentPath) {
  auto shader = std::make_unique<Shader>(vertexPath, fragmentPath);
  unsigned shaderId = shader->getShaderProgram();
  shaders[shaderId] = std::move(shader);
  return shaderId;
}

Shader &ResourceManager::getShaderById(unsigned shaderId) { return *shaders.at(shaderId); }
