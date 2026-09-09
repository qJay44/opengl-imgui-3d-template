#pragma once

#include "../core/EngineContext.hpp"
#include "../gfx/AssetManager.hpp"
#include "../gfx/Render.hpp"
#include "ComponentPool.hpp"
#include "Entity.hpp"
#include "MultiView.hpp"

namespace ecs {

class Registry {
public:
  [[nodiscard]] Entity createEntity() {
    return nextEntity++;
  }

  template<typename T>
  void emplace(Entity entity, T component) {
    getPool<T>()->emplace(entity, std::move(component));
  }

  template<typename T>
  [[nodiscard]] T& get(Entity entity) {
    return getPool<T>()->get(entity);
  }

  template<typename T>
  [[nodiscard]] bool has(Entity entity) {
    getPool<T>()->has(entity);
  }

  template<typename... Components>
  [[nodiscard]] auto view() {
    std::vector<IComponentPool*> pools = {getPool<Components>()...};
    return MultiView<Components...>(pools);
  }

  [[nodiscard]] core::EngineContext* getEngineContext() {
    assert(ctx);
    return ctx;
  }

  [[nodiscard]] gfx::Renderer* getRenderer() {
    assert(renderer);
    return renderer;
  }

  [[nodiscard]] gfx::AssetManager* getAssetManager() {
    assert(assetManager);
    return assetManager;
  }

  void setEngineContext(core::EngineContext* ctx) {
    assert(!this->ctx);
    this->ctx = ctx;
  }

  void setRenderer(gfx::Renderer* renderer) {
    assert(!this->renderer);
    this->renderer = renderer;
  }

  void setAssetManager(gfx::AssetManager* assetManager) {
    assert(!this->assetManager);
    this->assetManager = assetManager;
  }

private:
  Entity nextEntity = 0;
  core::EngineContext* ctx = nullptr;
  gfx::Renderer* renderer = nullptr;
  gfx::AssetManager* assetManager = nullptr;
  std::vector<std::unique_ptr<IComponentPool>> componentPool;

private:
  static size_t getComponentTypeID() {
    static size_t counter = 0;
    return counter++;
  }

  template<typename T>
  ComponentPool<T>* getPool() {
    static size_t typeID = getComponentTypeID();

    if (typeID >= componentPool.size())
      componentPool.resize(typeID + 1);

    if (!componentPool[typeID])
      componentPool[typeID] = std::make_unique<ComponentPool<T>>();

    return static_cast<ComponentPool<T>*>(componentPool[typeID].get());
  }
};

} // namespace ecs

