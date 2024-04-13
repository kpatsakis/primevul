void RenderWidgetHostViewAura::SetKeyboardFocus() {
#if defined(OS_WIN)
  if (CanFocus()) {
    aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
    if (dispatcher)
      ::SetFocus(dispatcher->host()->GetAcceleratedWidget());
  }
#endif
}
