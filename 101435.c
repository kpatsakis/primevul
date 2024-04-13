ResourceDispatcherHostImpl::CreateResourceHandlerForDownload(
    net::URLRequest* request,
    bool is_content_initiated,
    bool must_download,
    uint32_t id,
    scoped_ptr<DownloadSaveInfo> save_info,
    const DownloadUrlParameters::OnStartedCallback& started_cb) {
  scoped_ptr<ResourceHandler> handler(new DownloadResourceHandler(
      id, request, started_cb, std::move(save_info)));
  if (delegate_) {
    const ResourceRequestInfoImpl* request_info(
        ResourceRequestInfoImpl::ForRequest(request));

    ScopedVector<ResourceThrottle> throttles;
    delegate_->DownloadStarting(
        request, request_info->GetContext(), request_info->GetChildID(),
        request_info->GetRouteID(), request_info->GetRequestID(),
        is_content_initiated, must_download, &throttles);
    if (!throttles.empty()) {
      handler.reset(new ThrottlingResourceHandler(std::move(handler), request,
                                                  std::move(throttles)));
    }
  }
  return handler;
}
