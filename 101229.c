void Shell::OnShelfCreatedForRootWindow(aura::Window* root_window) {
  FOR_EACH_OBSERVER(ShellObserver,
                    observers_,
                    OnShelfCreatedForRootWindow(root_window));
}
