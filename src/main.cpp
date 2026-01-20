#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Camera/Camera.h"
#include "Lighting/Lighting.h"
#include "Lighting/presets/Flashlight.h"
#include "Lighting/presets/GlobalLight.h"
#include "Model/Model.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "vertices.h"

const char *TITLE = "Sandbox";
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

void framebufferSizeCallback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

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
  // glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glewExperimental = true;
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW.\n";
    return -1;
  }
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  stbi_set_flip_vertically_on_load(true);

  Shader shader(PROJECT_PATH + "/src/shaders/vertex.glsl", PROJECT_PATH + "/src/shaders/fragment.glsl");
  Shader lightShader(PROJECT_PATH + "/src/shaders/light.vertex.glsl", PROJECT_PATH + "/src/shaders/light.fragment.glsl");
  Texture metal(PROJECT_PATH + "/resources/textures/metal.jpg");
  Texture marble(PROJECT_PATH + "/resources/textures/marble.jpg");

  Lighting lighting;
  Types::DirectionalLight globalLight = GLOBAL_LIGHT;
  Types::SpotLight flashlight = FLASHLIGHT;
  lighting.addDirectionalLight(globalLight);
  lighting.addSpotLight(flashlight);
  lighting.uploadToShader(shader);

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

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    shader.use();
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    shader.setInt("textureDiffuse1", 0);
    shader.setInt("textureSpecular1", 0);

    flashlight.position = camera.getPosition();
    flashlight.direction = camera.getDirection();
    lighting.changeSpotLight(0, flashlight);
    lighting.uploadToShader(shader);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPosition", camera.getPosition());

    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, metal.getTexture());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, marble.getTexture());
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
