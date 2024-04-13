void Shell::ShowContextMenu(const gfx::Point& location_in_screen,
                            ui::MenuSourceType source_type) {
  if (!session_state_delegate_->NumberOfLoggedInUsers())
    return;
  if (session_state_delegate_->IsScreenLocked())
    return;

  aura::Window* root =
      wm::GetRootWindowMatching(gfx::Rect(location_in_screen, gfx::Size()));
  GetRootWindowController(root)
      ->ShowContextMenu(location_in_screen, source_type);
}
