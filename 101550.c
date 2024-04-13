void OmniboxViewViews::OnCompositingShuttingDown(ui::Compositor* compositor) {
  scoped_compositor_observer_.RemoveAll();
}
