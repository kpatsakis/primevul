void WebGLRenderingContextBase::TraceWrappers(
    ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(context_group_);
  visitor->TraceWrappers(bound_array_buffer_);
  visitor->TraceWrappers(renderbuffer_binding_);
  visitor->TraceWrappers(framebuffer_binding_);
  visitor->TraceWrappers(current_program_);
  visitor->TraceWrappers(bound_vertex_array_object_);
  for (auto& unit : texture_units_) {
    visitor->TraceWrappers(unit.texture2d_binding_);
    visitor->TraceWrappers(unit.texture_cube_map_binding_);
    visitor->TraceWrappers(unit.texture3d_binding_);
    visitor->TraceWrappers(unit.texture2d_array_binding_);
  }
  for (auto tracker : extensions_) {
    visitor->TraceWrappers(tracker);
  }
  CanvasRenderingContext::TraceWrappers(visitor);
}
