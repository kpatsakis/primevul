void Shell::OnMaximizeModeStarted() {
  FOR_EACH_OBSERVER(ShellObserver, observers_, OnMaximizeModeStarted());
}
