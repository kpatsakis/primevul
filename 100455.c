void GLES2DecoderImpl::DoEnable(GLenum cap) {
  SetCapabilityState(cap, true);
  glEnable(cap);
}
