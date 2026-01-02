#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"
#include "../third_party/stb/stb_image.h"
#include "Camera/Camera.h"
#include "Shader/Shader.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
};

const char *TITLE = "Sandbox";
const int WIDTH = 800, HEIGHT = 600;
std::string PROJECT_PATH = "/home/aminov/Documents/Programming/OpenGL/Sandbox";

bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

glm::vec3 lampPosition(1.2f, 1.5f, 2.0f);
glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
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
  glewExperimental = true;
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW.\n";
    return -1;
  }
  glViewport(0, 0, WIDTH, HEIGHT);

  Shader shader(PROJECT_PATH + "/src/shaders/vertex.glsl", PROJECT_PATH + "/src/shaders/fragment.glsl");
  Shader lampShader(PROJECT_PATH + "/src/shaders/lamp.vertex.glsl", PROJECT_PATH + "/src/shaders/lamp.fragment.glsl");

  unsigned VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned lampVAO;
  glGenVertexArrays(1, &lampVAO);
  glBindVertexArray(lampVAO);
  glBindBuffer(GL_VERTEX_ARRAY, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_VERTEX_ARRAY, 0);
  glBindVertexArray(0);

  shader.use();
  shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
  shader.setVec3("lightPosition", lampPosition);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    processInput(window);
    glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPosition", camera.getPosition());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    lampShader.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lampPosition);
    model = glm::scale(model, glm::vec3(0.2f));
    lampShader.setMat4("view", view);
    lampShader.setMat4("model", model);
    lampShader.setMat4("projection", projection);
    glBindVertexArray(lampVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}
