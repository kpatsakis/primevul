void FrameLoader::continueLoadAfterWillSubmitForm()
{
    if (!m_provisionalDocumentLoader)
        return;

    m_provisionalDocumentLoader->prepareForLoadStart();
    
    if (!m_provisionalDocumentLoader)
        return;

    DocumentLoader* activeDocLoader = activeDocumentLoader();
    if (activeDocLoader && activeDocLoader->isLoadingMainResource())
        return;

    m_loadingFromCachedPage = false;

    unsigned long identifier = 0;

    if (Page* page = m_frame->page()) {
        identifier = page->progress()->createUniqueIdentifier();
        notifier()->assignIdentifierToInitialRequest(identifier, m_provisionalDocumentLoader.get(), m_provisionalDocumentLoader->originalRequest());
    }

    if (!m_provisionalDocumentLoader->startLoadingMainResource(identifier))
        m_provisionalDocumentLoader->updateLoading();
}
