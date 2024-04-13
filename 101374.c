void RenderWidgetHostImpl::OnRenderProcessGone(int status, int exit_code) {
  if (!owned_by_render_frame_host_) {
    Destroy(true);
  } else {
    RendererExited(static_cast<base::TerminationStatus>(status), exit_code);
  }
}
