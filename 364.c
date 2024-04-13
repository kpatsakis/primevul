void WebProcessProxy::didNavigateWithNavigationData(uint64_t pageID, const WebNavigationDataStore& store, uint64_t frameID) 
{
    WebPageProxy* page = webPage(pageID);
    if (!page)
        return;
    
    WebFrameProxy* frame = webFrame(frameID);
    MESSAGE_CHECK(frame);
    MESSAGE_CHECK(frame->page() == page);
    
    m_context->historyClient().didNavigateWithNavigationData(m_context.get(), page, store, frame);
}
