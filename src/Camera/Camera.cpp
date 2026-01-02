#include "Camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &direction) : position(position), direction(direction) {
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  updateDirection();
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime, Direction movementDirection) {
  float velocity = speed * deltaTime;

  if (movementDirection == Direction::FORWARD) {
    position += direction * velocity;
  }
  if (movementDirection == Direction::BACK) {
    position -= direction * velocity;
  }
  if (movementDirection == Direction::LEFT) {
    position -= right * velocity;
  }
  if (movementDirection == Direction::RIGHT) {
    position += right * velocity;
  }
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
  xoffset *= SENSITIVITY;
  yoffset *= SENSITIVITY;
  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  updateDirection();
}

void Camera::processMouseScroll(float yoffset) {
  if (fov >= 1.0f && fov <= 45.0f) {
    fov -= yoffset;
  }
  if (fov < 1.0f) {
    fov = 1.0f;
  }
  if (fov > 45.0f) {
    fov = 45.0f;
  }
}

void Camera::updateDirection() {
  glm::vec3 tempDirection;
  tempDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  tempDirection.y = sin(glm::radians(pitch));
  tempDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction = glm::normalize(tempDirection);
  right = glm::normalize(glm::cross(direction, worldUp));
  up = glm::normalize(glm::cross(right, direction));
}

float Camera::getFov() { return fov; }
glm::vec3 Camera::getPosition() { return position; }
glm::mat4 Camera::getViewMatrix() { return glm::lookAt(position, position + direction, up); }
