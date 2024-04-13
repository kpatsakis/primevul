  explicit ScopedPixelUnpackBufferOverride(bool has_pixel_buffers,
                                           GLuint binding_override)
      : orig_binding_(-1) {
    if (has_pixel_buffers) {
      GLint orig_binding = 0;
      glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &orig_binding);
      if (static_cast<GLuint>(orig_binding) != binding_override) {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, binding_override);
        orig_binding_ = orig_binding;
      }
    }
  }
