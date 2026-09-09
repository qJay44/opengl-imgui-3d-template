#pragma once

namespace core {

struct EngineContext {
  GLFWwindow* window = nullptr;
  double dt = 1e-6;
  double time = 0.0;
  bool keyboardKeys[GLFW_KEY_LAST]{};

  inline ivec2 getWinSize() const {
    ivec2 res;
    glfwGetWindowSize(window, &res.x, &res.y);
    return res;
  }

  inline dvec2 getWinCenter() const {
    return dvec2(getWinSize()) * 0.5;
  }

  inline dvec2 getMousePos() const {
    dvec2 res;
    glfwGetCursorPos(window, &res.x, &res.y);
    return res;
  }

  inline float getAspectRatio() const {
    vec2 winSize = getWinSize();
    return winSize.x / winSize.y;
  }
};

} // namespace core

