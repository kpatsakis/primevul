PassRefPtr<WebPageProxy> WebProcessProxy::createWebPage(PageClient* pageClient, WebContext*, WebPageGroup* pageGroup)
{
    uint64_t pageID = generatePageID();
    RefPtr<WebPageProxy> webPage = WebPageProxy::create(pageClient, this, pageGroup, pageID);
    m_pageMap.set(pageID, webPage.get());
    globalPageMap().set(pageID, webPage.get());
#if PLATFORM(MAC)
    if (pageIsProcessSuppressible(webPage.get()))
        m_processSuppressiblePages.add(pageID);
    updateProcessSuppressionState();
#endif
    return webPage.release();
}
