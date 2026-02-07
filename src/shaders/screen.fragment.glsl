#version 330 core

in vec2 vertexTextureCoordinates;
out vec4 color;
uniform sampler2D screenTexture;

void main() {
  color = texture(screenTexture, vertexTextureCoordinates);
}
