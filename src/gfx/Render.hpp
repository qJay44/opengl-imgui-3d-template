#pragma once

#include "mesh/Mesh.hpp"
#include "../core/EngineContext.hpp"

namespace gfx {

class Renderer {
public:
  Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = default;

  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = default;

  ~Renderer() = default;

  void init(const core::EngineContext *ctx) const;
  void beginFrame() const;
  void submit(const gfx::Mesh* mesh) const;
  void endFrame(const core::EngineContext& ctx) const;
};

} // namespace gfx

