#include "RenderSystem.hpp"

#include "../components/MeshComponent.hpp"
#include "../components/CameraComponent.hpp"
#include "../components/TransformComponent.hpp"
#include "../../core/EngineContext.hpp"
#include "../../gfx/Render.hpp"
#include "TransformSystem.hpp"
#include "CameraSystem.hpp"

namespace ecs::RenderSystem {

using namespace ecs::component;

void render(entt::registry& registry) {
  auto& ctx = registry.ctx().get<core::EngineContext>();
  auto& renderer = registry.ctx().get<gfx::Renderer>();

  auto camView = registry.view<CameraComponent, TransformComponent>();

  [[maybe_unused]] core::Camera* activeCam = nullptr;
  vec3 activeCamPos{};
  for (auto entity : camView) {
    const auto& camComponent = registry.get<CameraComponent>(entity);
    if (camComponent.isActive) {
      const auto& transComponent = registry.get<TransformComponent>(entity);

      activeCam = camComponent.cam;
      activeCamPos = transComponent.pos;
      break;
    }
  }

  assert(activeCam);

  auto meshView = registry.view<MeshComponent, CameraComponent, TransformComponent>();

  renderer.beginFrame();

  for (auto& entity : meshView) {
    const auto& meshComponent = registry.get<MeshComponent>(entity);
    const auto& camComponent = registry.get<CameraComponent>(entity);
    const auto& transComponent = registry.get<TransformComponent>(entity);

    meshComponent.shader->use();
    meshComponent.shader->setUniformMatrix4f("u_model", TransformSystem::getModel(transComponent));
    meshComponent.shader->setUniform3f("u_camPos", activeCamPos);
    CameraSystem::setCommonUniforms(camComponent, meshComponent.shader);

    renderer.submit(meshComponent.mesh);
  }

  renderer.endFrame(ctx);
}

}; // namespace RenderSystem

