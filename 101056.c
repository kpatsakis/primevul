LocalFrame* ResourceFetcher::frame() const
{
    if (m_documentLoader)
        return m_documentLoader->frame();
    if (m_document && m_document->importsController())
        return m_document->importsController()->master()->frame();
    return 0;
}
