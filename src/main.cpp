#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Camera/Camera.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f  //
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //
    glm::vec3(2.0f, 5.0f, -15.0f),   //
    glm::vec3(-1.5f, -2.2f, -2.5f),  //
    glm::vec3(-3.8f, -2.0f, -12.3f), //
    glm::vec3(2.4f, -0.4f, -3.5f),   //
    glm::vec3(-1.7f, 3.0f, -7.5f),   //
    glm::vec3(1.3f, -2.0f, -2.5f),   //
    glm::vec3(1.5f, 2.0f, -2.5f),    //
    glm::vec3(1.5f, 0.2f, -1.5f),    //
    glm::vec3(-1.3f, 1.0f, -1.5f),   //
};

const char *TITLE = "Sandbox";
const int WIDTH = 900, HEIGHT = 600;
std::string PROJECT_PATH = "/home/aminov/Documents/Programming/OpenGL/Engine";

bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

// glm::vec3 lightPosition(2.0f, 0.0f, 2.0f);
float lightOuterCutOff = 18.0f;
float lightCutOff = 13.0f;
float lightConstant = 1.0f;
float lightLinear = 0.09f;
float lightQuadratic = 0.032f;

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
  // Shader lightShader(PROJECT_PATH + "/src/shaders/light.vertex.glsl", PROJECT_PATH + "/src/shaders/light.fragment.glsl");

  shader.use();
  shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setInt("material.diffuse", 0);
  shader.setInt("material.specular", 1);
  shader.setFloat("material.shininess", 32.0f);
  shader.setFloat("light.constant", lightConstant);
  shader.setFloat("light.linear", lightLinear);
  shader.setFloat("light.quadratic", lightQuadratic);
  shader.setFloat("light.cutOff", cos(glm::radians(lightCutOff)));
  shader.setFloat("light.outerCutOff", cos(glm::radians(lightOuterCutOff)));
  shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
  shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
  shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  unsigned VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // unsigned lightVAO;
  // glGenVertexArrays(1, &lightVAO);
  // glBindVertexArray(lightVAO);
  // glBindBuffer(GL_VERTEX_ARRAY, VBO);
  // glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(0 * sizeof(float)));
  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_VERTEX_ARRAY, 0);
  // glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  Texture containerDiffuseMap(PROJECT_PATH + "/assets/containerDiffuseMap.png");
  Texture containerSpecularMap(PROJECT_PATH + "/assets/containerSpecularMap.png");
  // float angle = 0.0f;
  // float radius = 5.0f;
  // float speed = 1.0f;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // angle += speed * deltaTime;
    // float x = radius * cos(angle);
    // float z = radius * sin(angle);
    // lightPosition = glm::vec3(x, lightPosition.y, z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerDiffuseMap.getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, containerSpecularMap.getTexture());

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader.use();
    shader.setVec3("light.position", camera.getPosition());
    shader.setVec3("light.direction", camera.getDirection());
    shader.setVec3("viewPosition", camera.getPosition());
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      float angle = 20.0f * i;
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    // lightShader.use();
    // model = glm::mat4(1.0f);
    // model = glm::translate(model, lightPosition);
    // model = glm::scale(model, glm::vec3(0.2f));
    // lightShader.setMat4("view", view);
    // lightShader.setMat4("model", model);
    // lightShader.setMat4("projection", projection);
    // glBindVertexArray(lightVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}
