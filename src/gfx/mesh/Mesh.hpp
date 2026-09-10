#pragma once

#include <variant>

#include "BufferObject.hpp"
#include "VAO.hpp"
#include "draw.hpp"
#include "MeshData.hpp"

namespace gfx {

class Mesh {
public:
  enum PolygonMode {
    POINT = GL_POINT,
    LINE = GL_LINE,
    FILL = GL_FILL,
  };

  Mesh() = default;
  Mesh(const Mesh&) = delete;
  Mesh(Mesh&&) = default;

  Mesh& operator=(const Mesh&) = delete;
  Mesh& operator=(Mesh&&) = default;
  ~Mesh() = default;

  VAO vao{};
  BufferObject vbo{GL_ARRAY_BUFFER};
  BufferObject ebo{GL_ELEMENT_ARRAY_BUFFER};
  PolygonMode polygonMode = FILL;

  std::variant<ArraysDraw, ElementsDraw> drawCmd;

private:
  friend class AssetManager;

  [[nodiscard]] Mesh(const MeshData& data);

  void linkAttributes(const vertex::Layout& layout);
};

} // namespace gfx

