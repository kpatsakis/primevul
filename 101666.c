void RenderWidgetHostImpl::NotifyScreenInfoChanged() {
  SynchronizeVisualProperties();

  if (delegate_ && !delegate_->IsWidgetForMainFrame(this))
    return;

  if (auto* touch_emulator = GetExistingTouchEmulator())
    touch_emulator->SetDeviceScaleFactor(GetScaleFactorForView(view_.get()));
}
