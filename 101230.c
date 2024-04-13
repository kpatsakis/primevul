void Shell::ShowAppList(aura::Window* window) {
  if (!window)
    window = GetTargetRootWindow();
  if (!app_list_controller_)
    app_list_controller_.reset(new AppListController);
  app_list_controller_->Show(window);
}
