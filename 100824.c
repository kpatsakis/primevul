void DevToolsWindow::SetWindowBounds(int x, int y, int width, int height) {
  if (!IsDocked())
    browser_->window()->SetBounds(gfx::Rect(x, y, width, height));
}
