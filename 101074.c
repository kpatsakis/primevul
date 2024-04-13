void RenderFrameHostImpl::SetPendingShutdown(const base::Closure& on_swap_out) {
  render_view_host_->SetPendingShutdown(on_swap_out);
}
