#pragma once

class Buffers final {
public:
  Buffers();
  Buffers(Buffers &) = delete;
  ~Buffers();

  unsigned getCubeVertexArray();
  unsigned getPlaneVertexArray();
  unsigned getSkyboxVertexArray();
  unsigned getScreenVertexArray();
  unsigned getSingleColorCubeVertexArray();
  unsigned getMaticesUniformBuffer();

private:
  unsigned cubeVAO, cubeVBO;
  unsigned planeVAO, planeVBO;
  unsigned skyboxVAO, skyboxVBO;
  unsigned screenVAO, screenVBO;
  unsigned singleColorCubeVAO;
  unsigned matricesUBO;
};
