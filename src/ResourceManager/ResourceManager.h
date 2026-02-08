#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Shader/Shader.h"
#include "../Texture/Texture.h"

class ResourceManager final {
public:
  ResourceManager();
  ResourceManager(ResourceManager &) = delete;

  unsigned loadShader(std::string vertexPath, std::string fragmentPath);
  unsigned loadTexture(std::string path);
  unsigned loadTexture(std::vector<std::string> faces);

  Shader &getShaderById(unsigned shaderId);
  Texture &getTextureById(unsigned textureId);

private:
  std::unordered_map<unsigned, std::unique_ptr<Texture>> textures;
  std::unordered_map<unsigned, std::unique_ptr<Shader>> shaders;
};
