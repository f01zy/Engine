#pragma once

#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;
const float SENSITIVITY = 0.05f;
const float ACCELERATION = 1.0f;
const float STOPPING_ACCELERATION = 2.0f;
const float MAX_SPEED = 0.2f;

enum Direction { FORWARD, BACK, LEFT, RIGHT, UP, DOWN };

class Camera final {
public:
  Camera(glm::vec3 &position, glm::vec3 &direction);
  Camera(Camera &) = delete;

  glm::vec3 getDirection();
  glm::vec3 getPosition();
  glm::mat4 getViewMatrix();
  float getFov();

  void processMovement();
  void processKeyboard(GLFWwindow *window, float deltaTime, std::vector<Direction> movementDirections);
  void processMouseMovement(float xoffset, float yoffset);
  void processMouseScroll(float yoffset);

private:
  void updateDirection();

  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 right;
  glm::vec3 worldUp;
  glm::vec3 up;
  glm::vec3 velocity;

  float acceleration = ACCELERATION;
  float stoppingAcceleration = STOPPING_ACCELERATION;
  float maxSpeed = MAX_SPEED;
  float sensitivity = SENSITIVITY;
  float yaw = YAW;
  float pitch = PITCH;
  float fov = FOV;
};
