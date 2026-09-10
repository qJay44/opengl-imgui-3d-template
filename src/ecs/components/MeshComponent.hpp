#pragma once

#include "../../gfx/mesh/Mesh.hpp"
#include "../../gfx/Shader.hpp"

namespace ecs::component {

struct MeshComponent {
  gfx::Mesh* mesh;
  gfx::Shader* shader;
};

} // namespace ecs::component

