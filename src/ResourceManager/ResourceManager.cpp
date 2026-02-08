#include "ResourceManager.h"
#include <memory>

ResourceManager::ResourceManager() {}

unsigned ResourceManager::loadShader(std::string vertexPath, std::string fragmentPath) {
  auto shader = std::make_unique<Shader>(vertexPath, fragmentPath);
  unsigned shaderId = shader->getShaderProgram();
  shaders[shaderId] = std::move(shader);
  return shaderId;
}

unsigned ResourceManager::loadTexture(std::string path) {
  auto texture = std::make_unique<Texture>(path);
  unsigned textureId = texture->getTexture();
  textures[textureId] = std::move(texture);
  return textureId;
}

unsigned ResourceManager::loadTexture(std::vector<std::string> faces) {
  auto texture = std::make_unique<Texture>(faces);
  unsigned textureId = texture->getTexture();
  textures[textureId] = std::move(texture);
  return textureId;
}

Shader &ResourceManager::getShaderById(unsigned shaderId) { return *shaders.at(shaderId); }
Texture &ResourceManager::getTextureById(unsigned textureId) { return *textures.at(textureId); }
