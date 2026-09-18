#include "RenderSystem.hpp"

#include "../components/MeshComponent.hpp"
#include "../components/CameraComponent.hpp"
#include "../components/TransformComponent.hpp"
#include "../components/TextureComponent.hpp"
#include "../components/AuxiliaryComponent.hpp"
#include "../systems/CameraSystem.hpp"
#include "../../core/EngineContext.hpp"
#include "../../gfx/Renderer.hpp"
#include "../../gfx/AssetManager.hpp"
#include "TransformSystem.hpp"

namespace ecs::system::RenderSystem {

using namespace ecs::component;

void render(entt::registry& registry) {
  auto& ctx = registry.ctx().get<core::EngineContext>();
  auto& renderer = registry.ctx().get<gfx::Renderer>();
  auto& assetManager = registry.ctx().get<gfx::AssetManager>();

  core::Light* globalLight = assetManager.getLight("GlobalLight");

  assert(activeCam);
  assert(globalLight);

  // TODO: What about FBOs?
  renderer.beginFrame(ctx.getWinSize());
  renderer.setGlobalLight(globalLight);

  for (auto& entity : registry.view<MeshComponent, TransformComponent, TextureComponent, CameraComponent>()) {
    const auto& meshComponent = registry.get<MeshComponent>(entity);
    const auto& transComponent = registry.get<TransformComponent>(entity);
    const auto& textureComponent = registry.get<TextureComponent>(entity);
    const auto& camComponent = registry.get<CameraComponent>(entity);

    if (meshComponent.disabled)
      continue;

    gfx::Renderer::RenderCommand renderCmd{
      .shader = meshComponent.shader,
      .mesh = meshComponent.mesh,
      .textures = textureComponent.textures,
    };

    meshComponent.shader->setUniformMatrix4f("u_model", TransformSystem::getModel(transComponent));
    CameraSystem::setUniforms(camComponent, meshComponent.shader);

    renderer.submit(std::move(renderCmd));
  }

  for (auto& entity : registry.view<MeshComponent, TransformComponent, AuxiliaryComponent, CameraComponent>()) {
    const auto& meshComponent = registry.get<MeshComponent>(entity);
    const auto& transComponent = registry.get<TransformComponent>(entity);
    const auto& camComponent = registry.get<CameraComponent>(entity);

    if (meshComponent.disabled)
      continue;

    gfx::Renderer::RenderCommand renderCmd{
      .shader = meshComponent.shader,
      .mesh = meshComponent.mesh,
    };

    meshComponent.shader->setUniformMatrix4f("u_model", TransformSystem::getModel(transComponent));
    CameraSystem::setUniforms(camComponent, meshComponent.shader);

    renderer.submit(std::move(renderCmd));
  }

  renderer.endFrame(ctx);
}

}; // namespace RenderSystem

