void WebProcessProxy::shouldTerminate(bool& shouldTerminate)
{
    if (!m_pageMap.isEmpty() || (m_downloadProxyMap && !m_downloadProxyMap->isEmpty()) || !m_context->shouldTerminate(this)) {
        shouldTerminate = false;
        return;
    }

    shouldTerminate = true;

    disconnect();
}
