void RenderProcessHostImpl::CreateOffscreenCanvasProvider(
    blink::mojom::OffscreenCanvasProviderRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!offscreen_canvas_provider_) {
    uint32_t renderer_client_id = base::checked_cast<uint32_t>(id_);
    offscreen_canvas_provider_ = std::make_unique<OffscreenCanvasProviderImpl>(
        GetHostFrameSinkManager(), renderer_client_id);
  }
  offscreen_canvas_provider_->Add(std::move(request));
}
