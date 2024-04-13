void RenderWidgetHostViewAura::SoftwareFrameWasFreed(
    uint32 output_surface_id, unsigned frame_id) {
  ReleaseSoftwareFrame(output_surface_id, frame_id);
}
