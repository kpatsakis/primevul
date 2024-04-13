void WebProcessProxy::assumeReadAccessToBaseURL(const String& urlString)
{
    KURL url(KURL(), urlString);
    if (!url.isLocalFile())
        return;

    KURL baseURL(KURL(), url.baseAsString());
    
    m_localPathsWithAssumedReadAccess.add(baseURL.fileSystemPath());
}
