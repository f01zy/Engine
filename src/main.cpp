#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unordered_map>

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

std::unordered_map<int, Direction> movements{
    {GLFW_KEY_W, Direction::FORWARD}, {GLFW_KEY_S, Direction::BACK},   {GLFW_KEY_A, Direction::LEFT},
    {GLFW_KEY_D, Direction::RIGHT},   {GLFW_KEY_SPACE, Direction::UP}, {GLFW_KEY_LEFT_SHIFT, Direction::DOWN},
};

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  std::vector<Direction> directions;
  for (const auto &i : movements) {
    if (glfwGetKey(window, i.first) == GLFW_PRESS) {
      directions.push_back(i.second);
    }
  }
  if (directions.empty()) {
    camera.processKeyboard(window, deltaTime, {});
  } else {
    camera.processKeyboard(window, deltaTime, directions);
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
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  Shader baseShader(PROJECT_PATH + "/src/shaders/base.vertex.glsl", PROJECT_PATH + "/src/shaders/base.fragment.glsl");
  Shader singleColorShader(PROJECT_PATH + "/src/shaders/singleColor.vertex.glsl", PROJECT_PATH + "/src/shaders/singleColor.fragment.glsl");
  Shader screenShader(PROJECT_PATH + "/src/shaders/screen.vertex.glsl", PROJECT_PATH + "/src/shaders/screen.fragment.glsl");
  Shader skyboxShader(PROJECT_PATH + "/src/shaders/skybox.vertex.glsl", PROJECT_PATH + "/src/shaders/skybox.fragment.glsl");
  Texture metalTexture(PROJECT_PATH + "/resources/textures/metal.jpg");
  Texture marbleTexture(PROJECT_PATH + "/resources/textures/marble.jpg");
  std::vector<std::string> faces{PROJECT_PATH + "/resources/textures/skybox/right.jpg", PROJECT_PATH + "/resources/textures/skybox/left.jpg",
                                 PROJECT_PATH + "/resources/textures/skybox/top.jpg",   PROJECT_PATH + "/resources/textures/skybox/bottom.jpg",
                                 PROJECT_PATH + "/resources/textures/skybox/front.jpg", PROJECT_PATH + "/resources/textures/skybox/back.jpg"};
  Texture skybox(faces);
  Lighting lighting;

  Types::DirectionalLight globalLight = GLOBAL_LIGHT;
  Types::PointLight lamp = LAMP;
  Types::SpotLight flashlight = FLASHLIGHT;
  std::vector<glm::vec3> lamps{glm::vec3(1.5f, 1.5f, 1.5f)};
  std::vector<glm::vec3> cubes{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f, 0.0f, 0.0f)};

  for (const glm::vec3 &lampPosition : lamps) {
    lamp.position = lampPosition;
    lighting.addPointLight(lamp);
  }
  lighting.addDirectionalLight(globalLight);
  lighting.addSpotLight(flashlight);
  lighting.uploadToShader(baseShader);

  unsigned skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

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

  unsigned screenVAO, screenVBO;
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

  unsigned FBO;
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  unsigned screenTexture;
  glGenTextures(1, &screenTexture);
  glBindTexture(GL_TEXTURE_2D, screenTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
  unsigned RBO;
  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer is not complete!\n";
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    glDepthMask(false);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(true);
    glBindVertexArray(0);

    baseShader.use();
    flashlight.position = camera.getPosition();
    flashlight.direction = camera.getDirection();
    lighting.changeSpotLight(0, flashlight);
    lighting.uploadToShader(baseShader);

    baseShader.setMat4("view", view);
    baseShader.setMat4("projection", projection);
    baseShader.setVec3("viewPosition", camera.getPosition());
    baseShader.setMat4("model", model);

    glBindTexture(GL_TEXTURE_2D, metalTexture.getTexture());
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, marbleTexture.getTexture());
    glBindVertexArray(cubeVAO);
    for (const glm::vec3 &cubePosition : cubes) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(cubePosition));
      baseShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    singleColorShader.use();
    singleColorShader.setMat4("view", view);
    singleColorShader.setMat4("projection", projection);
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader.use();
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glBindVertexArray(screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteVertexArrays(1, &singleColorCubeVAO);
  glDeleteVertexArrays(1, &screenVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);
  glDeleteBuffers(1, &screenVBO);
  glDeleteBuffers(1, &skyboxVBO);
  glDeleteBuffers(1, &FBO);
  glfwTerminate();
}
