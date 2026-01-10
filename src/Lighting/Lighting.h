#pragma once

#include <glm.hpp>
#include <vector>

#include "../Shader/Shader.h"
#include "../types/CachedUniformNames.h"
#include "../types/Lighting.h"

class Lighting final {
public:
  Lighting();
  Lighting(Lighting &) = delete;

  void addDirectionalLight(const DirectionalLight &light);
  void addPointLight(const PointLight &light);
  void addSpotLight(const SpotLight &light);
  void changePointLight(unsigned id, const PointLight &light);
  void changeSpotLight(unsigned id, const SpotLight &light);
  void uploadToShader(Shader &shader);
  const std::vector<PointLight> &getPointLights();

private:
  DirectionalLight directionalLight;
  std::vector<PointLight> pointLights;
  std::vector<SpotLight> spotLights;
  std::vector<CachedPointLightUniformNames> cachedPointLightUniformNames;
  std::vector<CachedSpotLightUniformNames> cachedSpotLightUniformNames;

  void uploadDirectionalLight(Shader &shader);
  void uploadPointLight(Shader &shader, unsigned id);
  void uploadSpotLight(Shader &shader, unsigned id);
};
