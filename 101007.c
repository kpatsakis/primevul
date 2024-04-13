bool FrameView::scrollbarsCanBeActive() const
{
    if (m_frame->view() != this)
        return false;

    return !!m_frame->document();
}
