scoped_ptr<RenderWidgetHostIterator> RenderWidgetHost::GetRenderWidgetHosts() {
  scoped_ptr<RenderWidgetHostIteratorImpl> hosts(
      new RenderWidgetHostIteratorImpl());
  for (auto& it : g_routing_id_widget_map.Get()) {
    RenderWidgetHost* widget = it.second;

    RenderViewHost* rvh = RenderViewHost::From(widget);
    if (!rvh) {
      hosts->Add(widget);
      continue;
    }

    if (static_cast<RenderViewHostImpl*>(rvh)->is_active())
      hosts->Add(widget);
  }

  return std::move(hosts);
}
