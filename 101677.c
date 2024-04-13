  MockRenderWidgetHostImpl* CreateRenderWidgetHostForProcess(
      MockRenderProcessHost* process_host) {
    return MockRenderWidgetHostImpl::Create(render_widget_host_delegate(),
                                            process_host,
                                            process_host->GetNextRoutingID());
  }
