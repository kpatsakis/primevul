bool WebContentsImpl::CreateRenderViewForInitialEmptyDocument() {
  return CreateRenderViewForRenderManager(GetRenderViewHost(),
                                          MSG_ROUTING_NONE);
}
