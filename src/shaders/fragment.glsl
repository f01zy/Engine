#version 330 core

in vec3 fragmentPosition;
in vec3 vertexNormal;

out vec4 color;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;

void main()
{
  vec3 lightDirection = normalize(lightPosition - fragmentPosition);
  float diff = max(dot(normalize(vertexNormal), lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;

  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  float specularStrength = 0.5f;
  vec3 viewDirection = normalize(viewPosition - fragmentPosition);
  vec3 reflectDirection = reflect(-lightDirection, vertexNormal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}
