void WebProcessProxy::didClose(CoreIPC::Connection*)
{
    RefPtr<WebProcessProxy> protect(this);

    webConnection()->didClose();

    Vector<RefPtr<WebPageProxy> > pages;
    copyValuesToVector(m_pageMap, pages);

    disconnect();

    for (size_t i = 0, size = pages.size(); i < size; ++i)
        pages[i]->processDidCrash();

}
