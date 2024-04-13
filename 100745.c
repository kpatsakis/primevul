void FrameLoader::didFirstLayout()
{
    if (m_frame->page() && isBackForwardLoadType(m_loadType))
        history()->restoreScrollPositionAndViewState();
}
