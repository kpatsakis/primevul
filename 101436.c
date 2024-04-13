void ResourceDispatcherHostImpl::DidReceiveResponse(ResourceLoader* loader) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();
  net::URLRequest* request = loader->request();
  if (request->was_fetched_via_proxy() &&
      request->was_fetched_via_spdy() &&
      request->url().SchemeIs(url::kHttpScheme)) {
    scheduler_->OnReceivedSpdyProxiedHttpResponse(
        info->GetChildID(), info->GetRouteID());
  }

  if (request->response_info().async_revalidation_required) {
    DCHECK(async_revalidation_manager_);
    async_revalidation_manager_->BeginAsyncRevalidation(*request,
                                                        scheduler_.get());
  }

  int render_process_id, render_frame_host;
  if (!info->GetAssociatedRenderFrame(&render_process_id, &render_frame_host))
    return;

  if (info->IsDownload())
    return;

  scoped_ptr<ResourceRequestDetails> detail(new ResourceRequestDetails(
      request, GetCertID(request, info->GetChildID())));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &NotifyResponseOnUI,
          render_process_id, render_frame_host, base::Passed(&detail)));
}
