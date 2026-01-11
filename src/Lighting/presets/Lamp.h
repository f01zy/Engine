#pragma once

#include "../../types/Lighting.h"

const glm::vec3 LAMP_AMBIENT(0.05f, 0.05f, 0.05f);
const glm::vec3 LAMP_DIFFUSE(0.8f, 0.8f, 0.8f);
const glm::vec3 LAMP_SPECULAR(1.0f, 1.0f, 1.0f);
const glm::vec3 LAMP_POSITION(0.0f, 0.0f, 0.0f);
const float LAMP_CONSTANT = 1.0f;
const float LAMP_LINEAR = 0.09f;
const float LAMP_QUADRATIC = 0.032f;
const Types::PointLight LAMP{LAMP_POSITION, LAMP_AMBIENT, LAMP_DIFFUSE, LAMP_SPECULAR, LAMP_CONSTANT, LAMP_LINEAR, LAMP_QUADRATIC};
