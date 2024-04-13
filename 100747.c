void FrameLoader::startCheckCompleteTimer()
{
    if (!m_shouldCallCheckCompleted)
        return;
    if (m_checkTimer.isActive())
        return;
    m_checkTimer.startOneShot(0);
}
