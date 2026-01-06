#pragma once

#include <vector>

#include "../../third_party/glm/glm.hpp"
#include "../Shader/Shader.h"

enum LightType { DIRECTIONAL, POINT, SPOT };

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
  float constant, linear, quadratic;
};

struct SpotLight : Light {
  glm::vec3 direction, position;
  float constant, linear, quadratic;
  float cutOff, outerCutOff;
};

class Lighting final {
public:
  Lighting();
  Lighting(Lighting &) = delete;

  void uploadToShader(Shader &shader);
  void addLight(LightType type, const Light &light);
  void setLight(LightType type, unsigned id, const Light &light);
  const std::vector<PointLight> &getPointLights();

private:
  std::vector<PointLight> pointLights;
  std::vector<SpotLight> spotLights;
  DirectionalLight directionalLight;

  void uploadDirectionalLight(Shader &shader);
  void uploadPointLight(Shader &shader, unsigned id);
  void uploadSpotLight(Shader &shader, unsigned id);
};
