#include "RenderSystem.hpp"

#include "../components/MeshComponent.hpp"
#include "../components/CameraComponent.hpp"

namespace ecs::RenderSystem {

void render(Registry& registry) {
  core::EngineContext* ctx = registry.getEngineContext();

  auto view = registry.view<MeshComponent, CameraComponent>();
  const gfx::Renderer* renderer = registry.getRenderer();

  renderer->beginFrame();

  for (auto& entity : view) {
    auto& meshComponent = view.get(entity);
    // auto& transComponent = view.get(entity);
    meshComponent.shader->use();

    renderer->submit(meshComponent.mesh, meshComponent.shader);
  }

  renderer->endFrame(ctx);
}

}; // namespace RenderSystem

