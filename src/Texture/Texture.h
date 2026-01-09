#pragma once

#include <string>

#include "../../third_party/stb/stb_image.h"

class Texture final {
public:
  Texture(std::string path);
  Texture(Texture &) = delete;
  unsigned getTexture();

private:
  std::string path;
  unsigned texture;
};
