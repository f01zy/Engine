#include "types/Objects.h"
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Buffers/Buffers.h"
#include "Camera/Camera.h"
#include "Lighting/Lighting.h"
#include "Lighting/presets/Flashlight.h"
#include "Lighting/presets/GlobalLight.h"
#include "Lighting/presets/Lamp.h"
#include "Objects/Objects.h"
#include "ResourceManager/ResourceManager.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

const char *TITLE = "Engine";
const int WIDTH = 900, HEIGHT = 600;
std::string PROJECT_PATH = "/home/aminov/Documents/Programming/OpenGL/Engine";

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

Types::DirectionalLight globalLight = GLOBAL_LIGHT;
Types::PointLight lamp = LAMP;
Types::SpotLight flashlight = FLASHLIGHT;
std::vector<glm::vec3> cubes{glm::vec3(-1.0f, 0.01f, -1.0f), glm::vec3(2.0f, 0.01f, 0.0f)};
std::vector<glm::vec3> lamps{glm::vec3(1.5f, 1.5f, 1.5f)};
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
  camera.processKeyboard(window, deltaTime, directions);
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
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  ResourceManager resourceManager;
  Lighting lighting;
  Buffers buffers;
  Objects objects;

  unsigned baseShaderId = resourceManager.loadShader(PROJECT_PATH + "/src/shaders/base.vertex.glsl", PROJECT_PATH + "/src/shaders/base.fragment.glsl");
  unsigned screenShaderId = resourceManager.loadShader(PROJECT_PATH + "/src/shaders/screen.vertex.glsl", PROJECT_PATH + "/src/shaders/screen.fragment.glsl");
  unsigned skyboxShaderId = resourceManager.loadShader(PROJECT_PATH + "/src/shaders/skybox.vertex.glsl", PROJECT_PATH + "/src/shaders/skybox.fragment.glsl");
  unsigned singleColorShaderId =
      resourceManager.loadShader(PROJECT_PATH + "/src/shaders/singleColor.vertex.glsl", PROJECT_PATH + "/src/shaders/singleColor.fragment.glsl");

  Shader &baseShader = resourceManager.getShaderById(baseShaderId);
  Shader &screenShader = resourceManager.getShaderById(screenShaderId);

  std::vector<std::string> faces{
      PROJECT_PATH + "/resources/textures/skybox/right.jpg", PROJECT_PATH + "/resources/textures/skybox/left.jpg",
      PROJECT_PATH + "/resources/textures/skybox/top.jpg",   PROJECT_PATH + "/resources/textures/skybox/bottom.jpg",
      PROJECT_PATH + "/resources/textures/skybox/front.jpg", PROJECT_PATH + "/resources/textures/skybox/back.jpg",
  };
  Texture skyboxTexture(faces);
  Texture metalTexture(PROJECT_PATH + "/resources/textures/metal.jpg");
  Texture marbleTexture(PROJECT_PATH + "/resources/textures/marble.jpg");

  for (const glm::vec3 &lampPosition : lamps) {
    lamp.position = lampPosition;
    lighting.addPointLight(lamp);
  }
  lighting.addDirectionalLight(globalLight);
  lighting.addSpotLight(flashlight);
  lighting.uploadToShader(baseShader);

  Object plane;
  plane.position = glm::vec3(0.0f, 0.0f, 0.0f);
  plane.verticesCount = 6;
  plane.textureTarget = GL_TEXTURE_2D;
  plane.texture = metalTexture.getTexture();
  plane.renderFlags = DEPTH_TEST;
  plane.vertexArray = buffers.getPlaneVertexArray();
  objects.addObject(plane, baseShaderId);

  for (const glm::vec3 &cubePosition : cubes) {
    Object cube;
    cube.position = cubePosition;
    cube.verticesCount = 36;
    cube.textureTarget = GL_TEXTURE_2D;
    cube.texture = marbleTexture.getTexture();
    cube.renderFlags = DEPTH_TEST | CULL_FACE;
    cube.vertexArray = buffers.getCubeVertexArray();
    objects.addObject(cube, baseShaderId);
  }

  for (const glm::vec3 &lampPosition : lamps) {
    Object lamp;
    lamp.position = lampPosition;
    lamp.scale = glm::vec3(0.2f);
    lamp.verticesCount = 36;
    lamp.renderFlags = DEPTH_TEST | CULL_FACE | SCALE;
    lamp.vertexArray = buffers.getSingleColorCubeVertexArray();
    objects.addObject(lamp, singleColorShaderId);
  }

  Object skybox;
  skybox.position = glm::vec3(0.0f, 0.0f, 0.0f);
  skybox.verticesCount = 36;
  skybox.textureTarget = GL_TEXTURE_CUBE_MAP;
  skybox.texture = skyboxTexture.getTexture();
  skybox.renderFlags = DEPTH_TEST | DEPTH_LEQUAL | SEPARATE_MATRICES | NO_TRANSFORM | RENDER_IN_THE_END;
  skybox.vertexArray = buffers.getSkyboxVertexArray();
  objects.addObject(skybox, skyboxShaderId);

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    processInput(window);
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
    objects.render(resourceManager, buffers.getMaticesUniformBuffer(), view, projection);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader.use();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glBindVertexArray(buffers.getScreenVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
