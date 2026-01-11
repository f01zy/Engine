#pragma once

#include <string>

namespace Types {
struct CachedPointLightUniformNames {
  std::string position;
  std::string ambient;
  std::string diffuse;
  std::string specular;
  std::string constant;
  std::string linear;
  std::string quadratic;
};

struct CachedSpotLightUniformNames {
  std::string position;
  std::string direction;
  std::string ambient;
  std::string diffuse;
  std::string specular;
  std::string constant;
  std::string linear;
  std::string quadratic;
  std::string cutOff;
  std::string outerCutOff;
};
} // namespace Types
