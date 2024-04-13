void GLES2DecoderImpl::DoStencilMaskSeparate(GLenum face, GLuint mask) {
  if (face == GL_FRONT) {
    mask_stencil_front_ = mask;
  } else {
    mask_stencil_back_ = mask;
  }
  glStencilMaskSeparate(face, mask);
}
