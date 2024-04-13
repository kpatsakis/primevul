void WebProcessProxy::pagePreferencesChanged(WebKit::WebPageProxy *page)
{
#if PLATFORM(MAC)
    if (pageIsProcessSuppressible(page))
        m_processSuppressiblePages.add(page->pageID());
    else
        m_processSuppressiblePages.remove(page->pageID());
    updateProcessSuppressionState();
#else
    UNUSED_PARAM(page);
#endif
}
