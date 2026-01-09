#include <iostream>
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
void Lighting::addPointLight(const PointLight &light) { pointLights.push_back(light); }
void Lighting::addSpotLight(const SpotLight &light) { spotLights.push_back(light); }

void Lighting::changePointLight(unsigned id, const PointLight &light) {
  if (id > pointLights.size() - 1) {
    std::cerr << "Point light with this id doesnt exists: " << id << "\n";
    return;
  }
  pointLights[id] = light;
}
void Lighting::changeSpotLight(unsigned id, const SpotLight &light) {
  if (id > spotLights.size() - 1) {
    std::cerr << "Spot light with this id doesnt exists: " << id << "\n";
    return;
  }
  spotLights[id] = light;
}

const std::vector<PointLight> &Lighting::getPointLights() { return pointLights; }

void Lighting::uploadDirectionalLight(Shader &shader) {
  shader.setVec3("directionalLight.direction", directionalLight.direction);
  shader.setVec3("directionalLight.ambient", directionalLight.ambient);
  shader.setVec3("directionalLight.diffuse", directionalLight.diffuse);
  shader.setVec3("directionalLight.specular", directionalLight.specular);
}

void Lighting::uploadPointLight(Shader &shader, unsigned id) {
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

void Lighting::uploadSpotLight(Shader &shader, unsigned id) {
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
