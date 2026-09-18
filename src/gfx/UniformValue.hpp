#pragma once

namespace gfx {

struct UniformValue {
  GLint location;
  std::variant<mat4, mat3, mat2, vec4, vec3, vec2, ivec4, ivec3, ivec2, float, int, bool> data;
};

} // namespace gfx

