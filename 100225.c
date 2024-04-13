void FrameLoader::completed()
{
    RefPtr<Frame> protect(m_frame);
    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->redirectScheduler()->startTimer();
    if (Frame* parent = m_frame->tree()->parent())
        parent->loader()->checkCompleted();
    if (m_frame->view())
        m_frame->view()->maintainScrollPositionAtAnchor(0);
}
