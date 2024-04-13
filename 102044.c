void TestNavigationManager::DidStartNavigation(NavigationHandle* handle) {
  if (!ShouldMonitorNavigation(handle))
    return;

  handle_ = handle;
  std::unique_ptr<NavigationThrottle> throttle(
      new TestNavigationManagerThrottle(
          handle_, base::Bind(&TestNavigationManager::OnWillStartRequest,
                              weak_factory_.GetWeakPtr()),
          base::Bind(&TestNavigationManager::OnWillProcessResponse,
                     weak_factory_.GetWeakPtr())));
  handle_->RegisterThrottleForTesting(std::move(throttle));
}
