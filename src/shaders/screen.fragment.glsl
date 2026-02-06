#version 330 core

in vec2 vertexTextureCoordinates;
out vec4 color;
uniform sampler2D screenTexture;

const float offset = 1.0 / 1600.0;

void main() {
  vec2 offsets[9] = vec2[](
      vec2(-offset, offset),
      vec2(0.0f, offset),
      vec2(offset, offset),
      vec2(-offset, 0.0f),
      vec2(0.0f, 0.0f),
      vec2(offset, 0.0f),
      vec2(-offset, -offset),
      vec2(0.0f, -offset),
      vec2(offset, -offset)
    );

  float kernel[9] = float[](
      -1, -1, -1,
      -1, 9, -1,
      -1, -1, -1
    );

  vec3 sampleTexture[9];
  for (int i = 0; i < 9; i++) {
    sampleTexture[i] = vec3(texture(screenTexture, vertexTextureCoordinates.st + offsets[i]));
  }

  vec3 fragmentColor = vec3(0.0);
  for (int i = 0; i < 9; i++) {
    fragmentColor += sampleTexture[i] * kernel[i];
  }
  color = vec4(fragmentColor, 1.0);
}
