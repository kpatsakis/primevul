CachedResource* Cache::resourceForURL(const String& url)
{
    return m_resources.get(url);
}
