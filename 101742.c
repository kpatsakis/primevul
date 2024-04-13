std::unique_ptr<RenderFrameHostImpl> RenderFrameHostManager::CreateRenderFrame(
    SiteInstance* instance,
    bool hidden,
    int* view_routing_id_ptr) {
  int32_t widget_routing_id = MSG_ROUTING_NONE;
  RenderFrameProxyHost* proxy = GetRenderFrameProxyHost(instance);

  CHECK(instance);

  std::unique_ptr<RenderFrameHostImpl> new_render_frame_host;
  bool success = true;
  if (view_routing_id_ptr)
    *view_routing_id_ptr = MSG_ROUTING_NONE;

  CHECK_NE(render_frame_host_->GetSiteInstance(), instance);

  if (frame_tree_node_->parent() &&
      frame_tree_node_->parent()->current_frame_host()->GetSiteInstance() !=
          instance) {
    widget_routing_id = instance->GetProcess()->GetNextRoutingID();
  }

  new_render_frame_host = CreateRenderFrameHost(
      instance, MSG_ROUTING_NONE, MSG_ROUTING_NONE, widget_routing_id, hidden,
      false);
  RenderViewHostImpl* render_view_host =
      new_render_frame_host->render_view_host();

  new_render_frame_host->GetProcess()->AddPendingView();

  if (frame_tree_node_->IsMainFrame()) {
    success = InitRenderView(render_view_host, proxy);

    if (!render_view_host->GetWidget()->GetView())
      delegate_->CreateRenderWidgetHostViewForRenderManager(render_view_host);
  } else {
    DCHECK(render_view_host->IsRenderViewLive());
  }

  if (success) {
    if (frame_tree_node_->IsMainFrame()) {
      if (render_view_host->GetWidget()->GetView())
        render_view_host->GetWidget()->GetView()->Hide();
    }
    DCHECK(new_render_frame_host);
    success = InitRenderFrame(new_render_frame_host.get());
  }

  if (success) {
    if (view_routing_id_ptr)
      *view_routing_id_ptr = render_view_host->GetRoutingID();
  }

  if (success) {
    DCHECK(new_render_frame_host->GetSiteInstance() == instance);
    return new_render_frame_host;
  }
  return nullptr;
}
