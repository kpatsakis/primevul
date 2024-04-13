bool GLES2DecoderImpl::DoIsBuffer(GLuint client_id) {
  return GetBufferInfo(client_id) != NULL;
}
