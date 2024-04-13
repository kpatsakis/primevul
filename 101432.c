void ResourceDispatcherHostImpl::BeginSaveFile(const GURL& url,
                                               const Referrer& referrer,
                                               SaveItemId save_item_id,
                                               SavePackageId save_package_id,
                                               int child_id,
                                               int render_view_route_id,
                                               int render_frame_route_id,
                                               ResourceContext* context) {
  if (is_shutdown_)
    return;

  char url_buf[128];
  base::strlcpy(url_buf, url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);
  CHECK(ContainsKey(active_resource_contexts_, context));

  request_id_--;

  const net::URLRequestContext* request_context = context->GetRequestContext();
  bool known_proto =
      request_context->job_factory()->IsHandledURL(url);
  if (!known_proto) {
    NOTREACHED();
    return;
  }

  scoped_ptr<net::URLRequest> request(
      request_context->CreateRequest(url, net::DEFAULT_PRIORITY, NULL));
  request->set_method("GET");
  SetReferrerForRequest(request.get(), referrer);

  request->SetLoadFlags(net::LOAD_PREFERRING_CACHE);

  ResourceRequestInfoImpl* extra_info =
      CreateRequestInfo(child_id, render_view_route_id,
                        render_frame_route_id, false, context);
  extra_info->AssociateWithRequest(request.get());  // Request takes ownership.

  scoped_ptr<ResourceHandler> handler(new SaveFileResourceHandler(
      request.get(), save_item_id, save_package_id, child_id,
      render_frame_route_id, url, save_file_manager_.get()));

  BeginRequestInternal(std::move(request), std::move(handler));
}
