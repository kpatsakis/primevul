  void OnReceivedRoutingIDOnUI(int widget_routing_id) {
    routing_id_ = widget_routing_id;
    run_loop_.Quit();
  }
