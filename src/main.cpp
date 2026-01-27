#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <string>

#include "Camera/Camera.h"
#include "Lighting/Lighting.h"
#include "Lighting/presets/Flashlight.h"
#include "Lighting/presets/GlobalLight.h"
#include "Lighting/presets/Lamp.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "vertices.h"

const char *TITLE = "Engine";
const int WIDTH = 900, HEIGHT = 600;
std::string PROJECT_PATH = "/home/aminov/Documents/Programming/OpenGL/Engine";

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 position(0.0f, 0.0f, 3.0f);
glm::vec3 direction(0.0f, 0.0f, -1.0f);
Camera camera(position, direction);

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::FORWARD);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::BACK);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::LEFT);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::RIGHT);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::UP);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera.processKeyboard(window, deltaTime, Direction::DOWN);
  }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) { camera.processMouseScroll(yoffset); }

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window.\n";
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glewExperimental = true;
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW.\n";
    return -1;
  }
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  Shader baseShader(PROJECT_PATH + "/src/shaders/base.vertex.glsl", PROJECT_PATH + "/src/shaders/base.fragment.glsl");
  Shader singleColorShader(PROJECT_PATH + "/src/shaders/singleColor.vertex.glsl", PROJECT_PATH + "/src/shaders/singleColor.fragment.glsl");
  Shader transparentShader(PROJECT_PATH + "/src/shaders/transparent.vertex.glsl", PROJECT_PATH + "/src/shaders/transparent.fragment.glsl");
  Texture metalTexture(PROJECT_PATH + "/resources/textures/metal.jpg");
  Texture marbleTexture(PROJECT_PATH + "/resources/textures/marble.jpg");
  Texture windowTexture(PROJECT_PATH + "/resources/textures/window.png");
  Lighting lighting;

  Types::DirectionalLight globalLight = GLOBAL_LIGHT;
  Types::PointLight lamp = LAMP;
  Types::SpotLight flashlight = FLASHLIGHT;
  std::vector<glm::vec3> lamps{glm::vec3(1.5f, 1.5f, 1.5f)};
  std::vector<glm::vec3> cubes{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f, 0.0f, 0.0f)};
  std::vector<glm::vec3> windows{glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f), glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
                                 glm::vec3(0.5f, 0.0f, -0.6f)};

  lighting.addDirectionalLight(globalLight);
  lighting.addSpotLight(flashlight);
  for (const glm::vec3 &lampPosition : lamps) {
    lamp.position = lampPosition;
    lighting.addPointLight(lamp);
  }
  lighting.uploadToShader(baseShader);

  unsigned cubeVAO, cubeVBO;
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

  unsigned planeVAO, planeVBO;
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

  unsigned singleColorCubeVAO;
  glGenVertexArrays(1, &singleColorCubeVAO);
  glBindVertexArray(singleColorCubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  unsigned transparentVAO, transparentVBO;
  glGenVertexArrays(1, &transparentVAO);
  glGenBuffers(1, &transparentVBO);
  glBindVertexArray(transparentVAO);
  glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TRANSPARENT_VERTICES), TRANSPARENT_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    baseShader.use();
    flashlight.position = camera.getPosition();
    flashlight.direction = camera.getDirection();
    lighting.changeSpotLight(0, flashlight);
    lighting.uploadToShader(baseShader);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    baseShader.setMat4("view", view);
    baseShader.setMat4("projection", projection);
    baseShader.setVec3("viewPosition", camera.getPosition());

    glEnable(GL_CULL_FACE);

    glStencilMask(0x00);
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, metalTexture.getTexture());
    baseShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, marbleTexture.getTexture());
    for (const glm::vec3 &cubePosition : cubes) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(cubePosition));
      baseShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glDisable(GL_DEPTH_TEST);
    singleColorShader.use();
    singleColorShader.setMat4("view", view);
    singleColorShader.setMat4("projection", projection);
    singleColorShader.setVec3("objectColor", 0.04f, 0.28f, 0.26f);
    glBindVertexArray(singleColorCubeVAO);
    glm::vec3 outlineScale(1.1f, 1.1f, 1.1f);
    for (const glm::vec3 &cubePosition : cubes) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(cubePosition));
      model = glm::scale(model, outlineScale);
      singleColorShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    singleColorShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    glBindVertexArray(singleColorCubeVAO);
    for (const glm::vec3 &lampPosition : lamps) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, lampPosition);
      model = glm::scale(model, glm::vec3(0.2f));
      singleColorShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    std::map<float, glm::vec3> sortedTransparentWindows;
    for (const glm::vec3 &windowPosition : windows) {
      float distance = glm::length(camera.getPosition() - windowPosition);
      sortedTransparentWindows[distance] = windowPosition;
    }

    glDisable(GL_CULL_FACE);

    transparentShader.use();
    transparentShader.setMat4("view", view);
    transparentShader.setMat4("projection", projection);
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture.getTexture());
    for (std::map<float, glm::vec3>::reverse_iterator it = sortedTransparentWindows.rbegin(); it != sortedTransparentWindows.rend(); it++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, it->second);
      transparentShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteVertexArrays(1, &singleColorCubeVAO);
  glDeleteVertexArrays(1, &transparentVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);
  glDeleteBuffers(1, &transparentVBO);
  glfwTerminate();
}
