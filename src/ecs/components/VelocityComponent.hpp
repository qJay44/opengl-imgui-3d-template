#pragma once

namespace ecs::component {

struct VelocityComponent {
  vec3 velocity;
  float scale{0.1f};
};

} // namespace ecs::component

