#pragma once

#include "../../third_party/glm/glm/glm.hpp"

struct Light {
  unsigned id;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

struct DirectionalLight : Light {
  glm::vec3 direction;
};

struct PointLight : Light {
  glm::vec3 position;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight : Light {
  glm::vec3 direction;
  glm::vec3 position;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
};
