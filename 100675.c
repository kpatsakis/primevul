void WebContentsImpl::OnDidLoadResourceFromMemoryCache(
    const GURL& url,
    const std::string& security_info,
    const std::string& http_method,
    const std::string& mime_type,
    ResourceType::Type resource_type) {
  base::StatsCounter cache("WebKit.CacheHit");
  cache.Increment();

  int cert_id = 0;
  net::CertStatus cert_status = 0;
  int security_bits = -1;
  int connection_status = 0;
  DeserializeSecurityInfo(security_info, &cert_id, &cert_status,
                          &security_bits, &connection_status);
  LoadFromMemoryCacheDetails details(
      url, GetRenderProcessHost()->GetID(), cert_id, cert_status, http_method,
      mime_type, resource_type);

  controller_.ssl_manager()->DidLoadFromMemoryCache(details);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidLoadResourceFromMemoryCache(details));

  NotificationService::current()->Notify(
      NOTIFICATION_LOAD_FROM_MEMORY_CACHE,
      Source<NavigationController>(&controller_),
      Details<LoadFromMemoryCacheDetails>(&details));

  if (url.is_valid() && url.SchemeIsHTTPOrHTTPS()) {
    scoped_refptr<net::URLRequestContextGetter> request_context(
        resource_type == ResourceType::MEDIA ?
            GetBrowserContext()->GetMediaRequestContextForRenderProcess(
                GetRenderProcessHost()->GetID()) :
            GetBrowserContext()->GetRequestContextForRenderProcess(
                GetRenderProcessHost()->GetID()));
    BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&NotifyCacheOnIO, request_context, url, http_method));
  }
}
