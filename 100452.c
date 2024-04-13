void GLES2DecoderImpl::DoColorMask(
    GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
  mask_red_ = red;
  mask_green_ = green;
  mask_blue_ = blue;
  mask_alpha_ = alpha;
  glColorMask(red, green, blue, alpha);
}
