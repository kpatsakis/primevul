  FakeRenderWidgetHostViewAura(RenderWidgetHost* widget,
                               bool is_guest_view_hack)
      : RenderWidgetHostViewAura(widget,
                                 is_guest_view_hack,
                                 false /* is_mus_browser_plugin_guest */),
        is_guest_view_hack_(is_guest_view_hack),
        delegated_frame_host_client_(
            new FakeDelegatedFrameHostClientAura(this)) {
    InstallDelegatedFrameHostClient(
        this, base::WrapUnique(delegated_frame_host_client_));
    CreateNewRendererCompositorFrameSink();
  }
