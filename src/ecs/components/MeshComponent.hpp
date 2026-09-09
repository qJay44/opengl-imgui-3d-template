#pragma once

#include "../../gfx/mesh/Mesh.hpp"
#include "../../gfx/Shader.hpp"

struct MeshComponent {
  gfx::Mesh* mesh;
  gfx::Shader* shader;
};

