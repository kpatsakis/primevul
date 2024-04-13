  FramebufferManager::FramebufferInfo* GetFramebufferInfoForTarget(
      GLenum target) {
    FramebufferManager::FramebufferInfo* info = NULL;
    switch (target) {
      case GL_FRAMEBUFFER:
      case GL_DRAW_FRAMEBUFFER:
        info = bound_draw_framebuffer_;
        break;
      case GL_READ_FRAMEBUFFER:
        info = bound_read_framebuffer_;
        break;
      default:
        NOTREACHED();
        break;
    }
    return info;
  }
