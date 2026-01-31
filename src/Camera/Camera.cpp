#include <algorithm>

#include "Camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &direction) : position(position), direction(direction) {
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  updateDirection();
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime, Direction movementDirection) {
  if (movementDirection == Direction::NONE) {
    float currentSpeed = glm::length(velocity);
    if (currentSpeed != 0) {
      float newSpeed = std::max(0.0f, currentSpeed - acceleration);
      velocity = glm::normalize(velocity) * newSpeed;
    }
  }
  if (glm::length(velocity) > maxSpeed) {
    velocity = glm::normalize(velocity) * maxSpeed;
  }
  if (movementDirection == Direction::FORWARD) {
    velocity += direction * acceleration;
  } else if (movementDirection == Direction::BACK) {
    velocity -= direction * acceleration;
  } else if (movementDirection == Direction::LEFT) {
    velocity -= right * acceleration;
  } else if (movementDirection == Direction::RIGHT) {
    velocity += right * acceleration;
  } else if (movementDirection == Direction::UP) {
    velocity += worldUp * acceleration;
  } else if (movementDirection == Direction::DOWN) {
    velocity -= worldUp * acceleration;
  }
  position += velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
  xoffset *= SENSITIVITY;
  yoffset *= SENSITIVITY;
  yaw += xoffset;
  pitch += yoffset;
  pitch = std::clamp(pitch, -89.0f, 89.0f);
  updateDirection();
}

void Camera::processMouseScroll(float yoffset) {
  if (fov >= 1.0f && fov <= 45.0f) {
    fov -= yoffset;
  }
  fov = std::clamp(fov, 1.0f, 45.0f);
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
glm::vec3 Camera::getDirection() { return direction; }
glm::vec3 Camera::getPosition() { return position; }
glm::mat4 Camera::getViewMatrix() { return glm::lookAt(position, position + direction, up); }
