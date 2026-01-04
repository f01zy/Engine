#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Service/Service.h"
#include "Shader.h"

Shader::Shader(std::string vertexPath, std::string fragmentPath) : vertexPath(vertexPath), fragmentPath(fragmentPath) {
  shaderProgram = glCreateProgram();
  initializeShader(vertexPath, GL_VERTEX_SHADER);
  initializeShader(fragmentPath, GL_FRAGMENT_SHADER);
  glLinkProgram(shaderProgram);
};

void Shader::use() { glUseProgram(shaderProgram); };

void Shader::initializeShader(std::string path, const unsigned type) {
  Service service;

  std::string string;
  service.readTextFromFile(path, string);
  const char *source = string.c_str();

  unsigned shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success;
  char info[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    std::cerr << "Error shader compilation: " << path << "\n" << info << "\n";
    return;
  }

  std::cout << "Shader compilation successful: " << path << "\n";
  glAttachShader(shaderProgram, shader);
  glDeleteShader(shader);
}

unsigned Shader::getShaderProgram() { return shaderProgram; }

void Shader::setInt(const std::string &name, int x) { glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), x); }
void Shader::setFloat(const std::string &name, float x) { glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), x); }
void Shader::setVec3(const std::string &name, float x, float y, float z) { glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); }
void Shader::setVec3(const std::string &name, glm::vec3 vector) { glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), vector.x, vector.y, vector.z); }
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) { glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, false, &mat[0][0]); }
