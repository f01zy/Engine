#pragma once

#include <stb_image.h>
#include <string>
#include <vector>

class Texture final {
public:
  Texture(std::string path);
  Texture(std::vector<std::string> faces);
  Texture(Texture &) = delete;
  ~Texture();
  unsigned getTexture();

private:
  unsigned texture;
};
