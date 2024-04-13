void RenderWidgetHostViewAura::UnlockResources() {
  DCHECK(frame_provider_);
  delegated_frame_evictor_->UnlockFrame();
}
