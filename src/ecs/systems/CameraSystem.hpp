#pragma once

#include "../components/CameraComponent.hpp"
#include "../../gfx/Shader.hpp"

namespace ecs::system::CameraSystem {

void onMouseMove(entt::registry& registry, dvec2 mousePos);
void update(entt::registry& registry);
void setUniforms(const ecs::component::CameraComponent& camComponent, gfx::Shader* shader);

};

