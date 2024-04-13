DownloadProxy* WebProcessProxy::createDownloadProxy()
{
#if ENABLE(NETWORK_PROCESS)
    ASSERT(!m_context->usesNetworkProcess());
#endif

    if (!m_downloadProxyMap)
        m_downloadProxyMap = adoptPtr(new DownloadProxyMap(m_messageReceiverMap));

    return m_downloadProxyMap->createDownloadProxy(m_context.get());
}
