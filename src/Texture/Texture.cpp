#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Texture.h"

Texture::Texture(std::string path) : path(path) {
  std::cout << "Loading texture: " << path << "\n";
  int width, height, channels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  unsigned format;

  if (!data) {
    std::cout << "Texture failed to load at path: " << path << "\n";
    stbi_image_free(data);
  }

  switch (channels) {
  case 1:
    format = GL_RED;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned Texture::getTexture() { return texture; }
