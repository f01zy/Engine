#version 330 core

#define POINT_LIGHTS 1
#define SPOT_LIGHTS 1

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
};

out vec4 color;
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinates;
uniform vec3 viewPosition;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];
uniform DirectionalLight directionalLight;

vec3 calcLightCoefficient(vec3 lightDirection, vec3 viewDirection, vec3 normal, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular) {
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float objectDiffuse = max(dot(normal, lightDirection), 0.0);
  float objectSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0f);
  vec3 ambient = lightAmbient * vec3(texture(textureDiffuse1, vertexTextureCoordinates));
  vec3 diffuse = lightDiffuse * objectDiffuse * vec3(texture(textureDiffuse1, vertexTextureCoordinates));
  vec3 specular = lightSpecular * objectSpecular * vec3(texture(textureSpecular1, vertexTextureCoordinates));
  return ambient + diffuse + specular;
}

float calcAttenuation(vec3 lightPosition, float constant, float linear, float quadratic) {
  float lightDistance = length(lightPosition - vertexPosition);
  float attenuation = 1.0 / (constant + linear * lightDistance + quadratic * (lightDistance * lightDistance));
  return attenuation;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(-light.direction);
  vec3 lightCoefficient = calcLightCoefficient(lightDirection, viewDirection, normal, light.ambient, light.diffuse, light.specular);
  return lightCoefficient;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - vertexPosition);
  vec3 lightCoefficient = calcLightCoefficient(lightDirection, viewDirection, normal, light.ambient, light.diffuse, light.specular);
  float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);
  return lightCoefficient * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - vertexPosition);
  vec3 lightCoefficient = calcLightCoefficient(lightDirection, viewDirection, normal, light.ambient, light.diffuse, light.specular);
  float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
  return lightCoefficient * attenuation * intensity;
}

void main() {
  vec3 normal = normalize(vertexNormal);
  vec3 viewDirection = normalize(viewPosition - vertexPosition);
  vec3 result = calcDirectionalLight(directionalLight, normal, viewDirection);
  for (int i = 0; i < max(POINT_LIGHTS, SPOT_LIGHTS); i++) {
    if (i < POINT_LIGHTS) {
      result += calcPointLight(pointLights[i], normal, viewDirection);
    }
    if (i < SPOT_LIGHTS) {
      result += calcSpotLight(spotLights[i], normal, viewDirection);
    }
  }
  color = vec4(result, 1.0f);
}
