void WebProcessProxy::removeWebPage(uint64_t pageID)
{
    m_pageMap.remove(pageID);
    globalPageMap().remove(pageID);
#if PLATFORM(MAC)
    m_processSuppressiblePages.remove(pageID);
    updateProcessSuppressionState();
#endif
}
