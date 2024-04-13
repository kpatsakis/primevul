void ResourceFetcher::didLoadResource(Resource* resource)
{
    RefPtr<DocumentLoader> protectDocumentLoader(m_documentLoader);
    RefPtrWillBeRawPtr<Document> protectDocument(m_document.get());

    if (resource && resource->response().isHTTP() && ((!resource->errorOccurred() && !resource->wasCanceled()) || resource->response().httpStatusCode() == 304) && document()) {
        ResourceTimingInfoMap::iterator it = m_resourceTimingInfoMap.find(resource);
        if (it != m_resourceTimingInfoMap.end()) {
            RefPtr<ResourceTimingInfo> info = it->value;
            m_resourceTimingInfoMap.remove(it);
            populateResourceTiming(info.get(), resource, false);
            reportResourceTiming(info.get(), document(), resource->type() == Resource::MainResource);
        }
    }

    if (frame())
        frame()->loader().loadDone();
    scheduleDocumentResourcesGC();
}
