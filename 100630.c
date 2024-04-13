void GLES2DecoderImpl::SetGLError(GLenum error, const char* msg) {
  if (msg) {
    last_error_ = msg;
    if (log_synthesized_gl_errors()) {
      LOG(ERROR) << last_error_;
    }
    if (!msg_callback_.is_null()) {
      msg_callback_.Run(0, GLES2Util::GetStringEnum(error) + " : " + msg);
    }
  }
  error_bits_ |= GLES2Util::GLErrorToErrorBit(error);
}
