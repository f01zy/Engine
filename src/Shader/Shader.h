#pragma once

#include <string>

#include "../../third_party/glm/glm/glm.hpp"

class Shader {
public:
  Shader(std::string vertexPath, std::string fragmentPath);
  Shader(Shader &) = delete;
  void use();
  unsigned getShaderProgram();

  void setInt(const std::string &name, int x);
  void setFloat(const std::string &name, float x);
  void setVec3(const std::string &name, float x, float y, float z);
  void setVec3(const std::string &name, glm::vec3 vector);
  void setMat4(const std::string &name, const glm::mat4 &mat);

private:
  void initializeShader(std::string path, const unsigned type);
  std::string vertexPath;
  std::string fragmentPath;
  unsigned shaderProgram;
};
