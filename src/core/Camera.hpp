#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace core {

struct Camera {
  float nearPlane = 0.1f;
  float farPlane = 1000.f;
  float fov = glm::radians(45.f);
  vec3 up{0.f, 1.f, 0.f};
  vec3 orientation{0.f, 0.f, -1.f};

  mat4 getProjMatrix(float aspectRatio) {
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  }

  mat4 getViewMatrix(vec3 eye) {
    return glm::lookAt(eye, eye + orientation, up);
  }
};

} // namespace core

