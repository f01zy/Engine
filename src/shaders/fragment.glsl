#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
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
in vec2 vertexTextureCoordinates;

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

  vec3 diffuse = light.diffuse * objectDiffuse * vec3(texture(material.diffuse, vertexTextureCoordinates));
  vec3 specular = light.specular * objectSpecular * vec3(texture(material.specular, vertexTextureCoordinates));
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertexTextureCoordinates));
  vec3 result = (ambient + diffuse + specular) * objectColor;

  color = vec4(result, 1.0f);
}
