#pragma once

#include <stb_image.h>
#include <string>

class Texture final {
public:
  Texture(std::string path);
  Texture(Texture &) = delete;
  unsigned getTexture();

private:
  std::string path;
  unsigned texture;
};
