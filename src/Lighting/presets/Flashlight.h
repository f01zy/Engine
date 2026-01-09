#pragma once

#include "../../types/Lighting.h"

const glm::vec3 FLASHLIGHT_AMBIENT(0.0f, 0.0f, 0.0f);
const glm::vec3 FLASHLIGHT_DIFFUSE(1.0f, 1.0f, 1.0f);
const glm::vec3 FLASHLIGHT_SPECULAR(1.0f, 1.0f, 1.0f);
const glm::vec3 FLASHLIGHT_POSITION(0.0f, 0.0f, 0.0f);
const glm::vec3 FLASHLIGHT_DIRECTION(0.0f, 0.0f, 0.0f);
const float FLASHLIGHT_CONSTANT = 1.0f;
const float FLASHLIGHT_LINEAR = 0.09f;
const float FLASHLIGHT_QUADRATIC = 0.032f;
const float FLASHLIGHT_CUT_OFF = glm::cos(glm::radians(13.0f));
const float FLASHLIGHT_OUTER_CUT_OFF = glm::cos(glm::radians(16.0f));
const SpotLight FLASHLIGHT{0,
                           FLASHLIGHT_AMBIENT,
                           FLASHLIGHT_DIFFUSE,
                           FLASHLIGHT_SPECULAR,
                           FLASHLIGHT_DIRECTION,
                           FLASHLIGHT_POSITION,
                           FLASHLIGHT_CONSTANT,
                           FLASHLIGHT_LINEAR,
                           FLASHLIGHT_QUADRATIC,
                           FLASHLIGHT_CUT_OFF,
                           FLASHLIGHT_OUTER_CUT_OFF};
