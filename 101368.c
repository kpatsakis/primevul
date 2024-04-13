RenderWidgetHostImpl::GetAllRenderWidgetHosts() {
  scoped_ptr<RenderWidgetHostIteratorImpl> hosts(
      new RenderWidgetHostIteratorImpl());
  for (auto& it : g_routing_id_widget_map.Get())
    hosts->Add(it.second);

  return std::move(hosts);
}
