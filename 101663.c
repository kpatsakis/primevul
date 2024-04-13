void RenderWidgetHostImpl::GetContentRenderingTimeoutFrom(
    RenderWidgetHostImpl* other) {
  if (other->new_content_rendering_timeout_ &&
      other->new_content_rendering_timeout_->IsRunning()) {
    new_content_rendering_timeout_->Start(
        other->new_content_rendering_timeout_->GetCurrentDelay());
  }
}
