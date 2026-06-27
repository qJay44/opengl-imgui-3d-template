#pragma once

#include "texture/Texture.hpp"

struct FBO {
  GLsizei size = 0;
  GLuint id = 0;

  FBO(GLsizei size) {
    gen(size);
  }

  FBO(const FBO&) = delete;
  FBO& operator=(const FBO&) = delete;

  FBO(FBO&& other) {
    std::swap(id, other.id);
  }

  FBO& operator=(FBO&& other) {
    if (this != &other)
      std::swap(id, other.id);

    return *this;
  }

  ~FBO() {
    clear();
  }

  static void unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void gen(GLsizei size) {
    this->size = size;
    glGenFramebuffers(size, &id);
  }

  void bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  void attach2D(GLenum attachment, const Texture& tex) const {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.getId(), 0);
    unbind();
  }

  void clear() {
    if (id) glDeleteFramebuffers(size, &id);
    size = 0;
    id = 0;
  }
};

