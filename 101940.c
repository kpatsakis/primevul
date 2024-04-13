void NavigationRequest::OnWillProcessResponseChecksComplete(
    NavigationThrottle::ThrottleCheckResult result) {
  DCHECK(result.action() != NavigationThrottle::DEFER);

  if (result.action() == NavigationThrottle::PROCEED) {
    bool served_via_resource_dispatcher_host =
        !base::FeatureList::IsEnabled(network::features::kNetworkService);
    served_via_resource_dispatcher_host =
        served_via_resource_dispatcher_host &&
        !(blink::ServiceWorkerUtils::IsServicificationEnabled() &&
          response_->head.was_fetched_via_service_worker);

    if (is_download_ && !served_via_resource_dispatcher_host) {
      auto resource_request = std::make_unique<network::ResourceRequest>();
      resource_request->url = common_params_.url;
      resource_request->method = common_params_.method;
      resource_request->request_initiator = common_params_.initiator_origin;
      resource_request->referrer = common_params_.referrer.url;
      resource_request->has_user_gesture = common_params_.has_user_gesture;

      BrowserContext* browser_context =
          frame_tree_node_->navigator()->GetController()->GetBrowserContext();
      DownloadManagerImpl* download_manager = static_cast<DownloadManagerImpl*>(
          BrowserContext::GetDownloadManager(browser_context));
      download_manager->InterceptNavigation(
          std::move(resource_request), navigation_handle_->GetRedirectChain(),
          response_, std::move(url_loader_client_endpoints_),
          ssl_info_.has_value() ? ssl_info_->cert_status : 0,
          frame_tree_node_->frame_tree_node_id());

      OnRequestFailedInternal(
          network::URLLoaderCompletionStatus(net::ERR_ABORTED),
          false /*skip_throttles*/, base::nullopt /*error_page_content*/,
          false /*collapse_frame*/);
      return;
    }

    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      if (url_loader_client_endpoints_) {
        network::mojom::URLLoaderPtr url_loader(
            std::move(url_loader_client_endpoints_->url_loader));
        url_loader->ProceedWithResponse();
        url_loader_client_endpoints_->url_loader = url_loader.PassInterface();
      } else {
        loader_->ProceedWithResponse();
      }
    }
  }

  if (result.action() == NavigationThrottle::CANCEL_AND_IGNORE ||
      result.action() == NavigationThrottle::CANCEL ||
      !response_should_be_rendered_) {
    render_frame_host_ = nullptr;

    if (!response_should_be_rendered_) {
      OnRequestFailedInternal(
          network::URLLoaderCompletionStatus(net::ERR_ABORTED),
          true /* skip_throttles */, base::nullopt /* error_page_content */,
          false /* collapse_frame */);
      return;
    }

    DCHECK(result.action() == NavigationThrottle::CANCEL ||
           result.net_error_code() == net::ERR_ABORTED);
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(result.net_error_code()),
        true /* skip_throttles */, result.error_page_content(),
        false /* collapse_frame */);

    return;
  }

  if (result.action() == NavigationThrottle::BLOCK_RESPONSE) {
    DCHECK_EQ(net::ERR_BLOCKED_BY_RESPONSE, result.net_error_code());
    render_frame_host_ = nullptr;
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(result.net_error_code()),
        true /* skip_throttles */, result.error_page_content(),
        false /* collapse_frame */);
    return;
  }

  CommitNavigation();

}
