void DesktopNativeWidgetHelperAura::OnRootWindowResized(
    const aura::RootWindow* root,
    const gfx::Size& old_size) {
  DCHECK_EQ(root, root_window_.get());
  widget_->SetBounds(gfx::Rect(root->GetHostOrigin(),
                               root->GetHostSize()));
}
