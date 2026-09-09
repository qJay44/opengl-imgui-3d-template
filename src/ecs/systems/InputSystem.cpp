#include "InputSystem.hpp"

#include "utils/utils.hpp"

namespace ecs::InputSystem {

namespace {

Registry* getRegistryFromGLFW(GLFWwindow* window) {
  Registry* registry = static_cast<Registry*>(glfwGetWindowUserPointer(window));
  if (!registry)
    error("[InputSystem::keyCallback] registry is nullptr");

  return registry;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  Registry* registry = getRegistryFromGLFW(window);
  core::EngineContext* ctx = registry->getEngineContext();

  if (action == GLFW_PRESS)   ctx->keyboardKeys[key] = true;
  if (action == GLFW_RELEASE) ctx->keyboardKeys[key] = false;

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + 2 * (key == GLFW_KEY_R));
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  // Registry* registry = getRegistryFromGLFW(window);
  // core::EngineContext* ctx = registry->getEngineContext();
  // TODO: Move camera
}

} // namespace

void init(Registry& registry) {
  core::EngineContext* ctx = registry.getEngineContext();

  glfwSetKeyCallback(ctx->window, keyCallback);
  glfwSetScrollCallback(ctx->window, scrollCallback);
  glfwSetCursorPosCallback(ctx->window, cursorPosCallback);

  glfwSetWindowUserPointer(ctx->window, &registry);
}

void update(Registry& registry) {
  core::EngineContext* ctx = registry.getEngineContext();

  if (ctx->keyboardKeys[GLFW_KEY_ESCAPE])
    glfwSetWindowShouldClose(ctx->window, GLFW_TRUE);
}

} // namespace InputSystem

