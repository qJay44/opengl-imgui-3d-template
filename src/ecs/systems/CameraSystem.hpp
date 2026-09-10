#pragma once

#include "../components/CameraComponent.hpp"
#include "../../gfx/Shader.hpp"
#include "entt/entity/fwd.hpp"

namespace ecs::CameraSystem {

void onMouseMove(entt::registry& registry, dvec2 mousePos);
void update(entt::registry& registry);
void setCommonUniforms(const component::CameraComponent& cameraComponent, gfx::Shader* shader);

};

