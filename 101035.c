void GLSurfaceOzoneSurfacelessSurfaceImpl::SwapBuffersAsync(
    const SwapCompletionCallback& callback) {
  if (!images_[current_surface_]->ScheduleOverlayPlane(
          widget_, 0, OverlayTransform::OVERLAY_TRANSFORM_NONE,
          gfx::Rect(GetSize()), gfx::RectF(1, 1))) {
    callback.Run(gfx::SwapResult::SWAP_FAILED);
    return;
  }
  GLSurfaceOzoneSurfaceless::SwapBuffersAsync(callback);
  current_surface_ ^= 1;
  BindFramebuffer();
}
