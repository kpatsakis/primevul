void ResourceFetcher::didFinishLoading(const Resource* resource, double finishTime, int64_t encodedDataLength)
{
    TRACE_EVENT_ASYNC_END0("net", "Resource", resource);
    context().dispatchDidFinishLoading(m_documentLoader, resource->identifier(), finishTime, encodedDataLength);
}
