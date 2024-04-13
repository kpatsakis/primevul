WebProcessProxy::~WebProcessProxy()
{
    if (m_webConnection)
        m_webConnection->invalidate();
}
