static WebProcessProxy::WebPageProxyMap& globalPageMap()
{
    ASSERT(isMainThread());
    DEFINE_STATIC_LOCAL(WebProcessProxy::WebPageProxyMap, pageMap, ());
    return pageMap;
}
