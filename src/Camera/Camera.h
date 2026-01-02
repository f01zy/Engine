#pragma once

#include <GLFW/glfw3.h>

#include "../../third_party/glm/glm.hpp"
#include "../../third_party/glm/gtc/matrix_transform.hpp"
#include "../../third_party/glm/gtc/type_ptr.hpp"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;
const float SENSITIVITY = 0.05f;
const float SPEED = 3.0f;

enum Direction { FORWARD, BACK, LEFT, RIGHT };

class Camera final {
public:
  Camera(glm::vec3 &position, glm::vec3 &direction);
  Camera(Camera &) = delete;

  glm::vec3 getPosition();
  glm::mat4 getViewMatrix();
  float getFov();

  void processKeyboard(GLFWwindow *window, float deltaTime, Direction movementDirection);
  void processMouseMovement(float xoffset, float yoffset);
  void processMouseScroll(float yoffset);

private:
  void updateDirection();

  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 right;
  glm::vec3 worldUp;
  glm::vec3 up;

  float sensitivity = SENSITIVITY;
  float speed = SPEED;
  float yaw = YAW;
  float pitch = PITCH;
  float fov = FOV;
};
