#pragma once

#include "Entity.hpp"

namespace ecs {

class IComponentPool {
public:
  virtual ~IComponentPool() = default;
  virtual void erase(Entity entity) = 0;
  virtual bool has(Entity entity) const = 0;
  virtual size_t size() const = 0;
  virtual Entity get(size_t idx) const = 0;
};

template<typename T>
class ComponentPool : public IComponentPool {
public:
  std::vector<T> components;
  std::vector<Entity> denseToEntity;
  std::vector<size_t> entityToDense;

  void emplace(Entity entity, T&& component) {
    if (entity >= entityToDense.size())
      entityToDense.resize(entity + 1, INVALID_ENTITY);

    entityToDense[entity] = components.size();
    denseToEntity.push_back(entity);
    components.push_back(std::move(component));
  }

  T& get(Entity entity) {
    return components[entityToDense[entity]];
  }

  bool has(Entity entity) const override {
    if (entity >= entityToDense.size())
      return false;
    return entityToDense[entity] != INVALID_ENTITY;
  }

  void erase(Entity entity) override {
    if (!has(entity))
      return;

    size_t indexToRemove = entityToDense[entity];
    size_t lastIndex = components.size() - 1;

    components[indexToRemove] = std::move(components[lastIndex]);
    denseToEntity[indexToRemove] = denseToEntity[lastIndex];

    Entity movedEntity = denseToEntity[indexToRemove];
    entityToDense[movedEntity] = indexToRemove;

    entityToDense[entity] = INVALID_ENTITY;

    components.pop_back();
    denseToEntity.pop_back();
  }

  size_t size() const override {
    return components.size();
  }

  Entity get(size_t idx) const override {
    return denseToEntity[idx];
  }
};

} // namespace ecs

