#version 330 core

out vec4 color;
in vec2 vertexTextureCoordinates;
uniform sampler2D vertexTexture;

void main() {
  color = texture(vertexTexture, vertexTextureCoordinates);
}
