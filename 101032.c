bool GLSurfaceOzoneSurfacelessSurfaceImpl::OnMakeCurrent(GLContext* context) {
  DCHECK(!context_ || context == context_);
  context_ = context;
  if (!fbo_) {
    glGenFramebuffersEXT(1, &fbo_);
    if (!fbo_)
      return false;
    glGenTextures(arraysize(textures_), textures_);
    if (!CreatePixmaps())
      return false;
  }
  BindFramebuffer();
  glBindFramebufferEXT(GL_FRAMEBUFFER, fbo_);
  return SurfacelessEGL::OnMakeCurrent(context);
}
