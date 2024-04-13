void GLES2DecoderImpl::RestoreStateForSimulatedAttrib0() {
  const VertexAttribManager::VertexAttribInfo* info =
      vertex_attrib_manager_->GetVertexAttribInfo(0);
  const void* ptr = reinterpret_cast<const void*>(info->offset());
  BufferManager::BufferInfo* buffer_info = info->buffer();
  glBindBuffer(GL_ARRAY_BUFFER, buffer_info ? buffer_info->service_id() : 0);
  glVertexAttribPointer(
      0, info->size(), info->type(), info->normalized(), info->gl_stride(),
      ptr);
  if (info->divisor())
    glVertexAttribDivisorANGLE(0, info->divisor());
  glBindBuffer(GL_ARRAY_BUFFER,
               bound_array_buffer_ ? bound_array_buffer_->service_id() : 0);
}
