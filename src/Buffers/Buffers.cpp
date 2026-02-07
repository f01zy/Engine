#include <GL/glew.h>
#include <glm.hpp>

#include "Buffers.h"
#include "vertices.h"

Buffers::Buffers() {
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_VERTICES), PLANE_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  glGenVertexArrays(1, &singleColorCubeVAO);
  glBindVertexArray(singleColorCubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &screenVAO);
  glGenBuffers(1, &screenVBO);
  glBindVertexArray(screenVAO);
  glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  glGenBuffers(1, &matricesUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
}

Buffers::~Buffers() {
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteVertexArrays(1, &singleColorCubeVAO);
  glDeleteVertexArrays(1, &screenVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);
  glDeleteBuffers(1, &screenVBO);
  glDeleteBuffers(1, &skyboxVBO);
  glDeleteBuffers(1, &matricesUBO);
}

unsigned Buffers::getCubeVertexArray() { return cubeVAO; }
unsigned Buffers::getPlaneVertexArray() { return planeVAO; }
unsigned Buffers::getSkyboxVertexArray() { return skyboxVAO; }
unsigned Buffers::getScreenVertexArray() { return screenVAO; }
unsigned Buffers::getSingleColorCubeVertexArray() { return singleColorCubeVAO; }
unsigned Buffers::getMaticesUniformBuffer() { return matricesUBO; }
