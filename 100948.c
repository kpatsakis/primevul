void ResourceDispatcherHostImpl::BeginRequest(
    int request_id,
    const ResourceHostMsg_Request& request_data,
    IPC::Message* sync_result,  // only valid for sync
    int route_id) {
  ProcessType process_type = filter_->process_type();
  int child_id = filter_->child_id();

  char url_buf[128];
  base::strlcpy(url_buf, request_data.url.spec().c_str(), arraysize(url_buf));
  base::debug::Alias(url_buf);

  linked_ptr<ResourceLoader> deferred_loader;
  {
    LoaderMap::iterator it = pending_loaders_.find(
        GlobalRequestID(request_data.transferred_request_child_id,
                        request_data.transferred_request_request_id));
    if (it != pending_loaders_.end()) {
      if (it->second->is_transferring()) {
        deferred_loader = it->second;
        pending_loaders_.erase(it);
      } else {
        RecordAction(UserMetricsAction("BadMessageTerminate_RDH"));
        filter_->BadMessageReceived();
        return;
      }
    }
  }

  ResourceContext* resource_context = filter_->resource_context();
  CHECK(ContainsKey(active_resource_contexts_, resource_context));

  if (is_shutdown_ ||
      !ShouldServiceRequest(process_type, child_id, request_data)) {
    AbortRequestBeforeItStarts(filter_, sync_result, route_id, request_id);
    return;
  }

  const Referrer referrer(MaybeStripReferrer(request_data.referrer),
                          request_data.referrer_policy);

  if (delegate_ && !delegate_->ShouldBeginRequest(child_id,
                                                  route_id,
                                                  request_data.method,
                                                  request_data.url,
                                                  request_data.resource_type,
                                                  resource_context,
                                                  referrer)) {
    AbortRequestBeforeItStarts(filter_, sync_result, route_id, request_id);
    return;
  }

  int load_flags =
      BuildLoadFlagsForRequest(request_data, child_id, sync_result != NULL);

  scoped_ptr<net::URLRequest> new_request;
  net::URLRequest* request;
  if (deferred_loader.get()) {
    request = deferred_loader->request();

    deferred_loader->WillCompleteTransfer();
  } else {
    net::URLRequestContext* context =
        filter_->GetURLRequestContext(request_data.resource_type);
    new_request.reset(context->CreateRequest(request_data.url, NULL));
    request = new_request.get();

    request->set_method(request_data.method);
    request->set_first_party_for_cookies(request_data.first_party_for_cookies);
    request->set_referrer(referrer.url.spec());
    webkit_glue::ConfigureURLRequestForReferrerPolicy(request,
                                                      referrer.policy);
    net::HttpRequestHeaders headers;
    headers.AddHeadersFromString(request_data.headers);
    request->SetExtraRequestHeaders(headers);
  }


  request->set_load_flags(load_flags);

  request->set_priority(DetermineRequestPriority(request_data));

  if (request_data.request_body) {
    request->set_upload(make_scoped_ptr(
        request_data.request_body->ResolveElementsAndCreateUploadDataStream(
            filter_->blob_storage_context()->controller(),
            filter_->file_system_context(),
            BrowserThread::GetMessageLoopProxyForThread(BrowserThread::FILE))));
  }

  bool allow_download = request_data.allow_download &&
      ResourceType::IsFrame(request_data.resource_type);

  ResourceRequestInfoImpl* extra_info =
      new ResourceRequestInfoImpl(
          process_type,
          child_id,
          route_id,
          request_data.origin_pid,
          request_id,
          request_data.is_main_frame,
          request_data.frame_id,
          request_data.parent_is_main_frame,
          request_data.parent_frame_id,
          request_data.resource_type,
          request_data.transition_type,
          false,  // is download
          allow_download,
          request_data.has_user_gesture,
          request_data.referrer_policy,
          resource_context);
  extra_info->AssociateWithRequest(request);  // Request takes ownership.

  if (request->url().SchemeIs(chrome::kBlobScheme)) {
    extra_info->set_requested_blob_data(
        filter_->blob_storage_context()->controller()->
            GetBlobDataFromUrl(request->url()));
  }

  appcache::AppCacheInterceptor::SetExtraRequestInfo(
      request, filter_->appcache_service(), child_id,
      request_data.appcache_host_id, request_data.resource_type);

  scoped_ptr<ResourceHandler> handler;
  if (sync_result) {
    handler.reset(new SyncResourceHandler(
        filter_, request, sync_result, this));
  } else {
    handler.reset(new AsyncResourceHandler(
        filter_, route_id, request, this));
  }

  if (request_data.download_to_file) {
    handler.reset(
        new RedirectToFileResourceHandler(handler.Pass(), child_id, this));
  }

  if (request_data.resource_type == ResourceType::MAIN_FRAME &&
      process_type == PROCESS_TYPE_RENDERER &&
      CrossSiteRequestManager::GetInstance()->
          HasPendingCrossSiteRequest(child_id, route_id)) {
    handler.reset(new CrossSiteResourceHandler(handler.Pass(), child_id,
                                               route_id, request));
  }

  handler.reset(
      new BufferedResourceHandler(handler.Pass(), this, request));

  ScopedVector<ResourceThrottle> throttles;
  if (delegate_) {
    bool is_continuation_of_transferred_request =
        (deferred_loader.get() != NULL);

    delegate_->RequestBeginning(request,
                                resource_context,
                                filter_->appcache_service(),
                                request_data.resource_type,
                                child_id,
                                route_id,
                                is_continuation_of_transferred_request,
                                &throttles);
  }

  if (request->has_upload()) {
    throttles.push_back(new PowerSaveBlockResourceThrottle("Uploading data."));
  }

  if (request_data.resource_type == ResourceType::MAIN_FRAME) {
    throttles.insert(
        throttles.begin(),
        new TransferNavigationResourceThrottle(request));
  }

  if (!throttles.empty()) {
    handler.reset(
        new ThrottlingResourceHandler(handler.Pass(), child_id, request_id,
                                      throttles.Pass()));
  }

  if (deferred_loader.get()) {
    pending_loaders_[extra_info->GetGlobalRequestID()] = deferred_loader;
    deferred_loader->CompleteTransfer(handler.Pass());
  } else {
    BeginRequestInternal(new_request.Pass(), handler.Pass());
  }
}
