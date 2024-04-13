RenderViewHost* WebContentsImpl::GetRenderViewHost() const {
  return render_manager_.current_host();
}
