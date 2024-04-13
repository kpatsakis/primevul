void WebProcessProxy::didDestroyFrame(uint64_t frameID)
{
    ASSERT(isGoodKey<WebFrameProxyMap>(frameID));
    m_frameMap.remove(frameID);
}
