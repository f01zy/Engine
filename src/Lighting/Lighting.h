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

  void addDirectionalLight(const Types::DirectionalLight &light);
  void addPointLight(const Types::PointLight &light);
  void addSpotLight(const Types::SpotLight &light);
  void changePointLight(unsigned id, const Types::PointLight &light);
  void changeSpotLight(unsigned id, const Types::SpotLight &light);
  void uploadToShader(Shader &shader);
  const std::vector<Types::PointLight> &getPointLights();

private:
  Types::DirectionalLight directionalLight;
  std::vector<Types::PointLight> pointLights;
  std::vector<Types::SpotLight> spotLights;
  std::vector<Types::CachedPointLightUniformNames> cachedPointLightUniformNames;
  std::vector<Types::CachedSpotLightUniformNames> cachedSpotLightUniformNames;

  void uploadDirectionalLight(Shader &shader);
  void uploadPointLight(Shader &shader, unsigned id);
  void uploadSpotLight(Shader &shader, unsigned id);
};
