void ResourceFetcher::incrementRequestCount(const Resource* res)
{
    if (res->ignoreForRequestCount())
        return;

    ++m_requestCount;
}
