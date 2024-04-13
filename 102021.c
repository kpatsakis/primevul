void RenderWidgetHostImpl::InitForFrame() {
  DCHECK(process_->HasConnection());
  renderer_initialized_ = true;

  if (view_)
    view_->OnRenderWidgetInit();
}
