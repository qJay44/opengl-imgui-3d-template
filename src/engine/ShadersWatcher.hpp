#pragma once

#include "Shader.hpp"
#include <vector>

class ShadersWatcher {
public:
  ShadersWatcher() = default;

  ShadersWatcher(ShadersWatcher&&) = default;
  ShadersWatcher(const ShadersWatcher&) = delete;

  ShadersWatcher& operator=(ShadersWatcher&&) = default;
  ShadersWatcher& operator=(const ShadersWatcher&) = delete;

  void add(Shader* shader) {
    shaders.push_back(shader);
  }

  void check() {
    for (Shader* shader : shaders)
      if (shader->needsReload())
        shader->reload();
  }

private:
  std::vector<Shader*> shaders;
};

