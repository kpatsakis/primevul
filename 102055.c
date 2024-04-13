void TestNavigationManager::OnWillStartRequest() {
  current_state_ = NavigationState::STARTED;
  navigation_paused_ = true;
  OnNavigationStateChanged();
}
