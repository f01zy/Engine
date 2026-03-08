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
#include "Text/Text.h"

const int WIDTH = 900, HEIGHT = 600;
const char *TITLE = "Engine";
std::string SHADERS_PATH = "/home/aminov/Documents/Programming/Engine/src/shaders";
std::string RESOURCES_PATH = "/home/aminov/Documents/Programming/Engine/resources";

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

bool firstMouse = true;
bool isWireframeMode = false;
bool isWireframeModeKeyPressed = false;
bool isDebugMode = false;
bool isDebugModeKeyPressed = false;

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

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !isWireframeModeKeyPressed) {
    isWireframeMode = !isWireframeMode;
    isWireframeModeKeyPressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
    isWireframeModeKeyPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !isDebugModeKeyPressed) {
    isDebugMode = !isDebugMode;
    isDebugModeKeyPressed = true;
  } else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) {
    isDebugModeKeyPressed = false;
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

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  ResourceManager resourceManager;
  Lighting lighting;
  Buffers buffers;
  Objects objects;

  unsigned textShaderId = resourceManager.loadShader(SHADERS_PATH + "/text.vertex.glsl", SHADERS_PATH + "/text.fragment.glsl");
  unsigned baseShaderId = resourceManager.loadShader(SHADERS_PATH + "/base.vertex.glsl", SHADERS_PATH + "/base.fragment.glsl");
  unsigned screenShaderId = resourceManager.loadShader(SHADERS_PATH + "/screen.vertex.glsl", SHADERS_PATH + "/screen.fragment.glsl");
  unsigned skyboxShaderId = resourceManager.loadShader(SHADERS_PATH + "/skybox.vertex.glsl", SHADERS_PATH + "/skybox.fragment.glsl");
  unsigned singleColorShaderId = resourceManager.loadShader(SHADERS_PATH + "/singleColor.vertex.glsl", SHADERS_PATH + "/singleColor.fragment.glsl");

  Shader &baseShader = resourceManager.getShaderById(baseShaderId);
  Shader &screenShader = resourceManager.getShaderById(screenShaderId);
  Shader &textShader = resourceManager.getShaderById(textShaderId);

  std::vector<std::string> faces{
      RESOURCES_PATH + "/textures/skybox/right.jpg",  RESOURCES_PATH + "/textures/skybox/left.jpg",  RESOURCES_PATH + "/textures/skybox/top.jpg",
      RESOURCES_PATH + "/textures/skybox/bottom.jpg", RESOURCES_PATH + "/textures/skybox/front.jpg", RESOURCES_PATH + "/textures/skybox/back.jpg",
  };
  unsigned skyboxTexture = resourceManager.loadTexture(faces);
  unsigned metalTexture = resourceManager.loadTexture(RESOURCES_PATH + "/textures/metal.jpg");
  unsigned marbleTexture = resourceManager.loadTexture(RESOURCES_PATH + "/textures/marble.jpg");
  Text text(RESOURCES_PATH + "/fonts/JetBrainsMono/ttf/JetBrainsMono-Bold.ttf");

  for (const glm::vec3 &lampPosition : lamps) {
    lamp.position = lampPosition;
    lighting.addPointLight(lamp);
  }
  lighting.addDirectionalLight(globalLight);
  lighting.addSpotLight(flashlight);
  lighting.uploadToShader(baseShader);

  for (const glm::vec3 &cubePosition : cubes) {
    Types::Object cube{
        cubePosition,
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.0f,
        GL_TEXTURE_2D,
        marbleTexture,
        buffers.getCubeVertexArray(),
        36,
        DEPTH_TEST | CULL_FACE,
    };
    objects.addObject(cube, baseShaderId);
  }

  for (const glm::vec3 &lampPosition : lamps) {
    Types::Object lamp{
        lampPosition,
        glm::vec3(0.2f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        0.0f,
        0,
        0,
        buffers.getSingleColorCubeVertexArray(),
        36,
        DEPTH_TEST | CULL_FACE | SCALE,
    };
    objects.addObject(lamp, singleColorShaderId);
  }

  Types::Object plane{
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      0.0f,
      GL_TEXTURE_2D,
      metalTexture,
      buffers.getPlaneVertexArray(),
      6,
      DEPTH_TEST,
  };
  objects.addObject(plane, baseShaderId);

  Types::Object skybox{
      glm::vec3(0.0f),
      glm::vec3(1.0f),
      glm::vec3(1.0f),
      glm::vec3(1.0f),
      0.0f,
      GL_TEXTURE_CUBE_MAP,
      skyboxTexture,
      buffers.getSkyboxVertexArray(),
      36,
      DEPTH_TEST | DEPTH_LEQUAL | SEPARATE_MATRICES | NO_TRANSFORM,
  };
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
    glm::vec3 position = camera.getPosition();
    glm::vec3 direction = camera.getDirection();
    flashlight.position = position;
    flashlight.direction = direction;
    lighting.changeSpotLight(0, flashlight);
    lighting.uploadToShader(baseShader);

    glPolygonMode(GL_FRONT_AND_BACK, isWireframeMode ? GL_LINE : GL_FILL);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    objects.render(resourceManager, buffers.getMaticesUniformBuffer(), view, projection);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    if (isDebugMode) {
      glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
      text.render(textShader, "FPS: ", 15.0f, HEIGHT - 25.0f, 0.3f, glm::vec3(1.0f), textProjection);
      text.render(textShader, std::to_string(static_cast<int>(1 / deltaTime)), 55.0f, HEIGHT - 25.0f, 0.3f, glm::vec3(1.0f), textProjection);
      text.render(textShader, "Position: ", 15.0f, HEIGHT - 50.0f, 0.3f, glm::vec3(1.0f), textProjection);
      text.render(textShader, std::to_string(position.x), 100.0f, HEIGHT - 50.0f, 0.3f, glm::vec3(1.0f), textProjection);
      text.render(textShader, std::to_string(position.y), 192.0f, HEIGHT - 50.0f, 0.3f, glm::vec3(1.0f), textProjection);
      text.render(textShader, std::to_string(position.z), 278.0f, HEIGHT - 50.0f, 0.3f, glm::vec3(1.0f), textProjection);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader.use();
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glBindVertexArray(buffers.getScreenVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
