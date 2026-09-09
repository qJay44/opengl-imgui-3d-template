#pragma once

#include "ComponentPool.hpp"
#include "Entity.hpp"
#include <cstddef>

namespace ecs {

template<typename... Components>
class MultiView {
public:
  MultiView(std::vector<IComponentPool*> pools) : pools(pools), size(pools.size()) {}

  class Iterator {
  public:
    Iterator(std::vector<IComponentPool*> allPools) : allPools(allPools), currPool(allPools.front()) {}

    Entity operator*() const { return currPool->get(currIdx); }

    Iterator& operator++() {
      ptr++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator&a, const Iterator&b) { return a == b; }
    friend bool operator!=(const Iterator&a, const Iterator&b) { return a != b; }

  private:
    std::vector<IComponentPool*> allPools;
    IComponentPool* currPool = nullptr;
    size_t currIdx = 0;
  };

  Iterator begin() { return Iterator(pools.data()); }
  Iterator end()   { return Iterator(pools.data() + size); }

private:
  std::vector<IComponentPool*> pools;
  size_t size = 0;
};

} // namespace ecs

