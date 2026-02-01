#include <algorithm>

#include "Camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &direction) : position(position), direction(direction) {
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  updateDirection();
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime, std::vector<Direction> movementDirections) {
  glm::vec3 movementDirectionVector(0.0f);
  for (const Direction &movementDirection : movementDirections) {
    if (movementDirection == Direction::FORWARD) {
      movementDirectionVector += direction;
    } else if (movementDirection == Direction::BACK) {
      movementDirectionVector -= direction;
    } else if (movementDirection == Direction::RIGHT) {
      movementDirectionVector += right;
    } else if (movementDirection == Direction::LEFT) {
      movementDirectionVector -= right;
    } else if (movementDirection == Direction::UP) {
      movementDirectionVector += worldUp;
    } else if (movementDirection == Direction::DOWN) {
      movementDirectionVector -= worldUp;
    }
  }
  if (glm::length(movementDirectionVector) == 0) {
    float currentSpeed = glm::length(velocity);
    if (currentSpeed != 0) {
      float newSpeed = std::max(0.0f, currentSpeed - stoppingAcceleration * deltaTime);
      velocity = glm::normalize(velocity) * newSpeed;
    }
    return;
  }
  velocity += glm::normalize(movementDirectionVector) * acceleration * deltaTime;
  if (glm::length(velocity) > maxSpeed) {
    velocity = glm::normalize(velocity) * maxSpeed;
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
