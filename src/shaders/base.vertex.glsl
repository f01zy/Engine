#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTextureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vertexPosition = vec3(model * vec4(position, 1.0));
  vertexNormal = mat3(transpose(inverse(model))) * normal;
  vertexTextureCoordinates = textureCoordinates;
  gl_Position = projection * view * model * vec4(position, 1.0);
}
