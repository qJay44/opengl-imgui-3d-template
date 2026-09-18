#include "CameraSystem.hpp"

#include "../../core/EngineContext.hpp"
#include "../components/TransformComponent.hpp"

namespace ecs::system::CameraSystem {

using namespace ecs::component;

void onMouseMove(entt::registry& registry, dvec2 mousePos) {
  const auto& ctx = registry.ctx().get<core::EngineContext>();

  core::Camera* activeCam = nullptr;
  for (auto entity : registry.view<CameraComponent>()) {
    const auto& camComponent = registry.get<CameraComponent>(entity);
    if (camComponent.isActive) {
      activeCam = camComponent.cam;
      break;
    }
  }

  assert(activeCam);

  dvec2 winSize = ctx.getWinSize();
  dvec2 winCenter = winSize * 0.5;
  dvec2 distFromCenter = mousePos - winCenter;

  dvec2 delta = dvec2(activeCam->sensitivity) * distFromCenter / winCenter;
  activeCam->yaw += delta.x;
  activeCam->pitch = glm::clamp(activeCam->pitch -= delta.y, -PI_2 + 0.1f, PI_2 - 0.1f);
}

void update(entt::registry& registry) {
  const auto& ctx = registry.ctx().get<core::EngineContext>();
  const float aspectRatio = ctx.getAspectRatio_WidthOverHeight();

  for (auto entity : registry.view<CameraComponent>()) {
    auto& camComponent = registry.get<CameraComponent>(entity);

    if (camComponent.isDetached) {
      auto& transComponent = registry.get<TransformComponent>(entity);
      camComponent.cam->position = transComponent.pos;
      camComponent.cam->update(aspectRatio);
    } else {
      camComponent.cam->update(aspectRatio);
    }
  }
}

void setUniforms(const CameraComponent& camComponent, gfx::Shader* shader) {
  shader->setUniformMatrix4f("u_proj", camComponent.cam->cachedProj);
  shader->setUniformMatrix4f("u_view", camComponent.cam->cachedView);
  shader->setUniform3f("u_camUp", camComponent.cam->up);
  shader->setUniform3f("u_camForward", camComponent.cam->orientation);
  shader->setUniform3f("u_camPos", camComponent.cam->position);
  shader->setUniform1f("u_camNear", camComponent.cam->nearPlane);
  shader->setUniform1f("u_camFar", camComponent.cam->farPlane);
  shader->setUniform1f("u_camFov", camComponent.cam->fov);
}

}; // namespace ecs::CameraSystem

