void FrameLoader::didExplicitOpen()
{
    m_isComplete = false;
    m_didCallImplicitClose = false;

    m_committedFirstRealDocumentLoad = true;
    
    m_frame->redirectScheduler()->cancel(); 
    if (m_frame->document()->url() != blankURL())
        m_URL = m_frame->document()->url();
}
