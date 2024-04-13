aura::Window* Shell::GetContainer(aura::Window* root_window,
                                  int container_id) {
  return root_window->GetChildById(container_id);
}
