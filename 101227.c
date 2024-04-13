aura::Window* Shell::GetTargetRootWindow() {
  CHECK(HasInstance());
  Shell* shell = GetInstance();
  if (shell->scoped_target_root_window_)
    return shell->scoped_target_root_window_;
  return shell->target_root_window_;
}
