  ProcessManagerBrowserTest() {
    guest_view::GuestViewManager::set_factory_for_testing(&factory_);
  }
