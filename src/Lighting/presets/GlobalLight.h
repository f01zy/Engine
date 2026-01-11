#pragma once

#include "../../types/Lighting.h"

const glm::vec3 GLOBAL_LIGHT_DIRECTION(-0.2f, -1.0f, -0.3f);
const glm::vec3 GLOBAL_LIGHT_AMBIENT(0.05f, 0.05f, 0.05f);
const glm::vec3 GLOBAL_LIGHT_DIFFUSE(0.4f, 0.4f, 0.4f);
const glm::vec3 GLOBAL_LIGHT_SPECULAR(0.5f, 0.5f, 0.5f);
const Types::DirectionalLight GLOBAL_LIGHT{GLOBAL_LIGHT_DIRECTION, GLOBAL_LIGHT_AMBIENT, GLOBAL_LIGHT_DIFFUSE, GLOBAL_LIGHT_SPECULAR};
