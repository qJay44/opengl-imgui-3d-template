#pragma once

#include <unordered_set>

#include "Shader.hpp"

struct ShadersWatcher {
  void add(Shader* shader) {
    shaders.insert(shader);
  }

  void check() {
    for (Shader* shader : shaders)
      if (shader->needsReload())
        shader->reload();
  }

private:
  std::unordered_set<Shader*> shaders;
};

