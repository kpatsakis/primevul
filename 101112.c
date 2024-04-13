void RenderWidgetHostViewAura::OnWindowTreeHostMoved(
    const aura::RootWindow* root,
    const gfx::Point& new_origin) {
  TRACE_EVENT1("ui", "RenderWidgetHostViewAura::OnWindowTreeHostMoved",
               "new_origin", new_origin.ToString());

  UpdateScreenInfo(window_);
}
