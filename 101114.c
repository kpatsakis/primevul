void RenderWidgetHostViewAura::RequestCopyOfOutput(
    scoped_ptr<cc::CopyOutputRequest> request) {
  window_->layer()->RequestCopyOfOutput(request.Pass());
}
