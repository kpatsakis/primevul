void ResourceFetcher::requestLoadStarted(Resource* resource, const FetchRequest& request, ResourceLoadStartType type)
{
    if (type == ResourceLoadingFromCache)
        notifyLoadedFromMemoryCache(resource);

    if (request.resourceRequest().url().protocolIsData() || (m_documentLoader && m_documentLoader->substituteData().isValid()))
        return;

    if (type == ResourceLoadingFromCache && !m_validatedURLs.contains(request.resourceRequest().url())) {
        RefPtr<ResourceTimingInfo> info = ResourceTimingInfo::create(request.options().initiatorInfo.name, monotonicallyIncreasingTime());
        populateResourceTiming(info.get(), resource, true);
        m_scheduledResourceTimingReports.add(info, resource->type() == Resource::MainResource);
        if (!m_resourceTimingReportTimer.isActive())
            m_resourceTimingReportTimer.startOneShot(0, FROM_HERE);
    }

    m_validatedURLs.add(request.resourceRequest().url());
}
