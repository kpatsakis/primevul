int WebContentsImpl::CreateOpenerRenderViewsForRenderManager(
    SiteInstance* instance) {
  if (!opener_)
    return MSG_ROUTING_NONE;

  return opener_->CreateOpenerRenderViews(instance);
}
