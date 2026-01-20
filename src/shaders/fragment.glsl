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
in vec3 fragmentPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinates;

uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform vec3 objectColor;
uniform vec3 viewPosition;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D textureDiffuse4;

uniform sampler2D textureSpecular1;
uniform sampler2D textureSpecular2;
uniform sampler2D textureSpecular3;
uniform sampler2D textureSpecular4;

vec3 calcLightCoefficient(vec3 lightDirection, vec3 normal, vec3 viewDirection, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular) {
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float objectDiffuse = max(dot(normal, lightDirection), 0.0);
  float objectSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0f);
  vec3 ambient = lightAmbient * vec3(texture(textureDiffuse1, vertexTextureCoordinates));
  vec3 diffuse = lightDiffuse * objectDiffuse * vec3(texture(textureDiffuse1, vertexTextureCoordinates));
  vec3 specular = lightSpecular * objectSpecular * vec3(texture(textureSpecular1, vertexTextureCoordinates));
  return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(-light.direction);
  vec3 lightProperties = calcLightCoefficient(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
  vec3 result = lightProperties * objectColor;
  return result;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragmentPosition);
  vec3 lightProperties = calcLightCoefficient(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
  float lightDistance = length(light.position - fragmentPosition);
  float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
  vec3 result = lightProperties * attenuation * objectColor;
  return result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragmentPosition);
  vec3 lightProperties = calcLightCoefficient(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
  float lightDistance = length(light.position - fragmentPosition);
  float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
  vec3 result = lightProperties * attenuation * intensity * objectColor;
  return result;
}

void main() {
  vec3 normal = normalize(vertexNormal);
  vec3 viewDirection = normalize(viewPosition - fragmentPosition);
  vec3 result = calcDirectionalLight(directionalLight, normal, viewDirection);
  for (int i = 0; i < POINT_LIGHTS; i++) {
    // result += calcPointLight(pointLights[i], normal, fragmentPosition, viewDirection);
  }
  for (int i = 0; i < SPOT_LIGHTS; i++) {
    result += calcSpotLight(spotLights[i], normal, fragmentPosition, viewDirection);
  }
  color = vec4(result, 1.0f);
}
