#include "Lighting.h"
#include <string>

Lighting::Lighting() {}

void Lighting::uploadToShader(Shader &shader) {
  shader.use();
  uploadDirectionalLight(shader);
  for (int i = 0; i < pointLights.size(); i++) {
    uploadPointLight(shader, i);
  }
  for (int i = 0; i < spotLights.size(); i++) {
    uploadSpotLight(shader, i);
  }
}

const std::vector<PointLight> &Lighting::getPointLights() { return pointLights; }

void Lighting::setDirectionalLight(const DirectionalLight &light) { directionalLight = light; }

void Lighting::addPointLight(const PointLight &light) { pointLights.push_back(light); }

void Lighting::addSpotLight(const SpotLight &light) { spotLights.push_back(light); }

void Lighting::setLightPosition(LightType type, int id, glm::vec3 position) {
  if (type == LightType::POINT) {
    pointLights[id].position = position;
  }
  if (type == LightType::SPOT) {
    spotLights[id].position = position;
  }
}

void Lighting::setLightDirection(LightType type, int id, glm::vec3 direction) {
  if (type == LightType::DIRECTIONAL) {
    directionalLight.direction = direction;
  }
  if (type == LightType::SPOT) {
    spotLights[id].direction = direction;
  }
}

void Lighting::uploadDirectionalLight(Shader &shader) {
  shader.setVec3("directionalLight.direction", directionalLight.direction);
  shader.setVec3("directionalLight.ambient", directionalLight.ambient);
  shader.setVec3("directionalLight.diffuse", directionalLight.diffuse);
  shader.setVec3("directionalLight.specular", directionalLight.specular);
}

void Lighting::uploadPointLight(Shader &shader, int id) {
  PointLight light = pointLights[id];
  std::string lightId = std::to_string(id);
  shader.setVec3("pointLights[" + lightId + "].position", light.position);
  shader.setVec3("pointLights[" + lightId + "].ambient", light.ambient);
  shader.setVec3("pointLights[" + lightId + "].diffuse", light.diffuse);
  shader.setVec3("pointLights[" + lightId + "].specular", light.specular);
  shader.setFloat("pointLights[" + lightId + "].constant", light.constant);
  shader.setFloat("pointLights[" + lightId + "].linear", light.linear);
  shader.setFloat("pointLights[" + lightId + "].quadratic", light.quadratic);
}

void Lighting::uploadSpotLight(Shader &shader, int id) {
  SpotLight light = spotLights[id];
  std::string lightId = std::to_string(id);
  shader.setVec3("spotLights[" + lightId + "].position", light.position);
  shader.setVec3("spotLights[" + lightId + "].direction", light.direction);
  shader.setVec3("spotLights[" + lightId + "].ambient", light.ambient);
  shader.setVec3("spotLights[" + lightId + "].diffuse", light.diffuse);
  shader.setVec3("spotLights[" + lightId + "].specular", light.specular);
  shader.setFloat("spotLights[" + lightId + "].constant", light.constant);
  shader.setFloat("spotLights[" + lightId + "].linear", light.linear);
  shader.setFloat("spotLights[" + lightId + "].quadratic", light.quadratic);
  shader.setFloat("spotLights[" + lightId + "].cutOff", light.cutOff);
  shader.setFloat("spotLights[" + lightId + "].outerCutOff", light.outerCutOff);
}
