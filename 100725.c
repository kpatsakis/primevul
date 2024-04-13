bool ScriptController::executeScriptIfJavaScriptURL(const KURL& url)
{
    if (!protocolIsJavaScript(url))
        return false;

    if (!m_frame->page()
        || !m_frame->document()->contentSecurityPolicy()->allowJavaScriptURLs(m_frame->document()->url(), eventHandlerPosition().m_line))
        return true;

    RefPtr<Frame> protector(m_frame);
    RefPtr<Document> ownerDocument(m_frame->document());

    const int javascriptSchemeLength = sizeof("javascript:") - 1;

    bool locationChangeBefore = m_frame->navigationScheduler()->locationChangePending();

    String decodedURL = decodeURLEscapeSequences(url.string());
    ScriptValue result = executeScript(decodedURL.substring(javascriptSchemeLength));

    if (!m_frame->page())
        return true;

    String scriptResult;
    if (!result.getString(scriptResult))
        return true;

    ASSERT(m_frame->document()->loader());

    if (!locationChangeBefore && m_frame->navigationScheduler()->locationChangePending())
        return true;

    if (RefPtr<DocumentLoader> loader = m_frame->document()->loader())
        loader->replaceDocument(scriptResult, ownerDocument.get());
    return true;
}
