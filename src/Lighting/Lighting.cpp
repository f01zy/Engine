#include <string>

#include "Lighting.h"

Lighting::Lighting() {}

void Lighting::uploadToShader(Shader &shader) {
  uploadDirectionalLight(shader);
  for (int i = 0; i < pointLights.size(); i++) {
    uploadPointLight(shader, i);
  }
  for (int i = 0; i < spotLights.size(); i++) {
    uploadSpotLight(shader, i);
  }
}

void Lighting::addDirectionalLight(const DirectionalLight &light) { directionalLight = light; }

void Lighting::addPointLight(const PointLight &light) {
  pointLights.push_back(light);
  unsigned id = pointLights.size() - 1;
  cachedPointLightUniformNames.push_back({
      "pointLights[" + std::to_string(id) + "].position",
      "pointLights[" + std::to_string(id) + "].ambient",
      "pointLights[" + std::to_string(id) + "].diffuse",
      "pointLights[" + std::to_string(id) + "].specular",
      "pointLights[" + std::to_string(id) + "].constant",
      "pointLights[" + std::to_string(id) + "].linear",
      "pointLights[" + std::to_string(id) + "].quadratic",
  });
}

void Lighting::addSpotLight(const SpotLight &light) {
  spotLights.push_back(light);
  unsigned id = spotLights.size() - 1;
  cachedSpotLightUniformNames.push_back({
      "spotLights[" + std::to_string(id) + "].position",
      "spotLights[" + std::to_string(id) + "].direction",
      "spotLights[" + std::to_string(id) + "].ambient",
      "spotLights[" + std::to_string(id) + "].diffuse",
      "spotLights[" + std::to_string(id) + "].specular",
      "spotLights[" + std::to_string(id) + "].constant",
      "spotLights[" + std::to_string(id) + "].linear",
      "spotLights[" + std::to_string(id) + "].quadratic",
      "spotLights[" + std::to_string(id) + "].cutOff",
      "spotLights[" + std::to_string(id) + "].outerCutOff",
  });
}

void Lighting::changePointLight(unsigned id, const PointLight &light) { pointLights[id] = light; }

void Lighting::changeSpotLight(unsigned id, const SpotLight &light) { spotLights[id] = light; }

const std::vector<PointLight> &Lighting::getPointLights() { return pointLights; }

void Lighting::uploadDirectionalLight(Shader &shader) {
  shader.setVec3("directionalLight.direction", directionalLight.direction);
  shader.setVec3("directionalLight.ambient", directionalLight.ambient);
  shader.setVec3("directionalLight.diffuse", directionalLight.diffuse);
  shader.setVec3("directionalLight.specular", directionalLight.specular);
}

void Lighting::uploadPointLight(Shader &shader, unsigned id) {
  PointLight &light = pointLights[id];
  CachedPointLightUniformNames &names = cachedPointLightUniformNames[id];
  shader.setVec3(names.position, light.position);
  shader.setVec3(names.ambient, light.ambient);
  shader.setVec3(names.diffuse, light.diffuse);
  shader.setVec3(names.specular, light.specular);
  shader.setFloat(names.constant, light.constant);
  shader.setFloat(names.linear, light.linear);
  shader.setFloat(names.quadratic, light.quadratic);
}

void Lighting::uploadSpotLight(Shader &shader, unsigned id) {
  SpotLight &light = spotLights[id];
  CachedSpotLightUniformNames &names = cachedSpotLightUniformNames[id];
  shader.setVec3(names.position, light.position);
  shader.setVec3(names.direction, light.direction);
  shader.setVec3(names.ambient, light.ambient);
  shader.setVec3(names.diffuse, light.diffuse);
  shader.setVec3(names.specular, light.specular);
  shader.setFloat(names.constant, light.constant);
  shader.setFloat(names.linear, light.linear);
  shader.setFloat(names.quadratic, light.quadratic);
  shader.setFloat(names.cutOff, light.cutOff);
  shader.setFloat(names.outerCutOff, light.outerCutOff);
}
