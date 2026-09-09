#include "Render.hpp"

#include "../core/EngineContext.hpp"
#include "utils/clrp.hpp"

namespace gfx {

namespace {

void GLAPIENTRY MessageCallback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
) {
  static const clrp::clrp_t clrpError{clrp::ATTRIBUTE::BOLD, clrp::FG::RED};
  static const clrp::clrp_t clrpWarning{clrp::ATTRIBUTE::BOLD, clrp::FG::YELLOW};

  clrp::clrp_t clrpFinal = clrpError;
  bool stop = true;

  switch (source) {
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      return; // Handled by the Shader class itself
  }

  // Suppress annoying SIMD32 callback
  if (type == GL_DEBUG_TYPE_PERFORMANCE) {
    clrpFinal = clrpWarning;
    stop = false;
  }

  fprintf(
    stderr, "GL CALLBACK: %s source = 0x%x, id = 0x%x type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), source, id, type, severity, clrp::format(message, clrpFinal).c_str()
  );

  if (stop)
    exit(EXIT_FAILURE);
}

} // namespace

void Renderer::init(const core::EngineContext* ctx) const {
  ivec2 winSize = ctx->getWinSize();

  glViewport(0, 0, winSize.x, winSize.y);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void Renderer::beginFrame() const {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);  // Disable for flat meshes, enable for volumetric meshes
  glEnable(GL_DEPTH_TEST); // Disable to ignore depth (draw one object over another one without discarding the farthest)
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + !global::wireframeMode); // Always use GL_FILL for fullscreen quads
}

void Renderer::submit(const gfx::Mesh* mesh, const gfx::Shader* shader, const mat4& model) const {
  mesh->vao.bind();

  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ArraysDraw>) {
      glDrawArrays(arg.mode, 0, arg.vertexCount);
    } else if constexpr (std::is_same_v<T, ElementsDraw>) {
      glDrawElements(arg.mode, arg.indexCount, arg.indexType, arg.indicesOffset);
    }
  }, mesh->drawCmd);
}

void Renderer::endFrame(const core::EngineContext* ctx) const {
  glfwSwapBuffers(ctx->window);
  glfwPollEvents();
}

} // namespace gfx

