#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 color;

in vec3 fragmentPosition;
in vec3 vertexNormal;

uniform Light light;
uniform Material material;
uniform vec3 objectColor;
uniform vec3 viewPosition;

void main()
{
  vec3 lightDirection = normalize(light.position - fragmentPosition);
  vec3 viewDirection = normalize(viewPosition - fragmentPosition);
  vec3 reflectDirection = reflect(-lightDirection, vertexNormal);

  float objectDiffuse = max(dot(normalize(vertexNormal), lightDirection), 0.0);
  float objectSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

  vec3 diffuse = light.diffuse * material.diffuse * objectDiffuse;
  vec3 specular = light.specular * material.specular * objectSpecular;
  vec3 ambient = light.ambient * material.ambient;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}
