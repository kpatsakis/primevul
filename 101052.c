void ResourceFetcher::didDownloadData(const Resource* resource, int dataLength, int encodedDataLength)
{
    context().dispatchDidDownloadData(m_documentLoader, resource->identifier(), dataLength, encodedDataLength);
}
