#version 330 core

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float cutOff;
  float outerCutOff;
  float constant;
  float linear;
  float quadratic;
};

out vec4 color;
in vec3 fragmentPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoordinates;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 objectColor;
uniform vec3 viewPosition;

vec3 calcLightProperties(vec3 lightDirection, vec3 normal, vec3 viewDirection, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular) {
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float objectDiffuse = max(dot(normal, lightDirection), 0.0);
  float objectSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  vec3 ambient = lightAmbient * vec3(texture(material.diffuse, vertexTextureCoordinates));
  vec3 diffuse = lightDiffuse * objectDiffuse * vec3(texture(material.diffuse, vertexTextureCoordinates));
  vec3 specular = lightSpecular * objectSpecular * vec3(texture(material.specular, vertexTextureCoordinates));
  return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(-light.direction);
  vec3 lightProperties = calcLightProperties(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
  vec3 result = lightProperties * objectColor;
  return result;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragmentPosition);
  vec3 lightProperties = calcLightProperties(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
  float lightDistance = length(light.position - fragmentPosition);
  float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
  vec3 result = lightProperties * attenuation * objectColor;
  return result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragmentPosition);
  vec3 lightProperties = calcLightProperties(lightDirection, normal, viewDirection, light.ambient, light.diffuse, light.specular);
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
  for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
    result += calcSpotLight(spotLights[i], normal, fragmentPosition, viewDirection);
  }
  for (int i = 0; i < NR_POINT_LIGHTS; i++) {
    result += calcPointLight(pointLights[i], normal, fragmentPosition, viewDirection);
  }
  color = vec4(result, 1.0f);
}
