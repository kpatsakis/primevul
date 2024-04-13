void ResourceDispatcherHostImpl::HandleSwapOutACK(
    const ViewMsg_SwapOut_Params& params, bool timed_out) {
  ResourceLoader* loader = GetLoader(params.new_render_process_host_id,
                                     params.new_request_id);
  if (loader) {
    ResourceRequestInfoImpl* info = loader->GetRequestInfo();
    if (info->cross_site_handler())
      info->cross_site_handler()->ResumeResponse();
  }

  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&OnSwapOutACKHelper,
                 params.closing_process_id,
                 params.closing_route_id,
                 timed_out));
}
