#pragma once

#include "mesh/Mesh.hpp"
#include "Shader.hpp"
#include "../core/EngineContext.hpp"

namespace gfx {

class Renderer {
public:
  void init(const core::EngineContext* ctx) const;
  void beginFrame() const;
  void submit(const gfx::Mesh* mesh, const gfx::Shader* shader, const mat4& model) const;
  void endFrame(const core::EngineContext* ctx) const;
};

} // namespace gfx

