#include "TimeSystem.hpp"

namespace ecs::TimeSystem {

void update(Registry& registry) {
  static double titleTimer = glfwGetTime();
  static double prevTime = titleTimer;
  static double currTime = prevTime;

  core::EngineContext& ctx = registry.getEngineContext();

  currTime = glfwGetTime();
  double dt = currTime - prevTime;

  // FPS cap
  // constexpr double fpsLimit = 1. / 90.;
  // if (dt < fpsLimit) continue;
  // else prevTime = currTime;

  ctx.dt = dt;
  ctx.time += dt;
}

}

