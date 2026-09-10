#include "TimeSystem.hpp"

#include "../../core/EngineContext.hpp"

namespace ecs::TimeSystem {

void update(entt::registry& registry) {
  static double titleTimer = glfwGetTime();
  static double prevTime = titleTimer;
  static double currTime = prevTime;

  auto& ctx = registry.ctx().get<core::EngineContext>();

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

