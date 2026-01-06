#pragma once

#include <vector>

#include "../../third_party/glm/glm.hpp"
#include "../Shader/Shader.h"

enum LightType { DIRECTIONAL, POINT, SPOT };

struct Light {
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
  void setDirectionalLight(const DirectionalLight &light);
  void addPointLight(const PointLight &light);
  void addSpotLight(const SpotLight &light);

  const std::vector<PointLight> &getPointLights();

  void setLightPosition(LightType type, int id, glm::vec3 position);
  void setLightDirection(LightType type, int id, glm::vec3 direction);

private:
  DirectionalLight directionalLight;
  std::vector<PointLight> pointLights;
  std::vector<SpotLight> spotLights;

  void uploadDirectionalLight(Shader &shader);
  void uploadPointLight(Shader &shader, int id);
  void uploadSpotLight(Shader &shader, int id);
};
