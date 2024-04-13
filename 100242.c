void FrameLoader::stopAllLoaders(DatabasePolicy databasePolicy)
{
    ASSERT(!m_frame->document() || !m_frame->document()->inPageCache());
    if (m_unloadEventBeingDispatched)
        return;

    if (m_inStopAllLoaders)
        return;

    m_inStopAllLoaders = true;

    policyChecker()->stopCheck();

    stopLoadingSubframes();
    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->stopLoading(databasePolicy);
    if (m_documentLoader)
        m_documentLoader->stopLoading(databasePolicy);

    setProvisionalDocumentLoader(0);
    
    if (m_documentLoader)
        m_documentLoader->clearArchiveResources();

    m_inStopAllLoaders = false;    
}
