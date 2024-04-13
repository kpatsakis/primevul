void WebProcessProxy::didUpdateHistoryTitle(uint64_t pageID, const String& title, const String& url, uint64_t frameID)
{
    WebPageProxy* page = webPage(pageID);
    if (!page)
        return;

    WebFrameProxy* frame = webFrame(frameID);
    MESSAGE_CHECK(frame);
    MESSAGE_CHECK(frame->page() == page);
    MESSAGE_CHECK_URL(url);

    m_context->historyClient().didUpdateHistoryTitle(m_context.get(), page, title, url, frame);
}
