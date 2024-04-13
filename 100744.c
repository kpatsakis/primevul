void FrameLoader::didExplicitOpen()
{
    m_isComplete = false;

    if (!m_stateMachine.committedFirstRealDocumentLoad())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);

    m_frame->navigationScheduler()->cancel();
}
