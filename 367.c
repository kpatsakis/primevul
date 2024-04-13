void WebProcessProxy::didReceiveMessage(CoreIPC::Connection* connection, CoreIPC::MessageID messageID, CoreIPC::MessageDecoder& decoder)
{
    if (m_messageReceiverMap.dispatchMessage(connection, messageID, decoder))
        return;

    if (m_context->dispatchMessage(connection, messageID, decoder))
        return;

    if (decoder.messageReceiverName() == Messages::WebProcessProxy::messageReceiverName()) {
        didReceiveWebProcessProxyMessage(connection, messageID, decoder);
        return;
    }

#if ENABLE(CUSTOM_PROTOCOLS)
    if (decoder.messageReceiverName() == Messages::CustomProtocolManagerProxy::messageReceiverName()) {
#if ENABLE(NETWORK_PROCESS)
        ASSERT(!context()->usesNetworkProcess());
#endif
        m_customProtocolManagerProxy.didReceiveMessage(connection, messageID, decoder);
        return;
    }
#endif

    uint64_t pageID = decoder.destinationID();
    if (!pageID)
        return;

    WebPageProxy* pageProxy = webPage(pageID);
    if (!pageProxy)
        return;
    
    pageProxy->didReceiveMessage(connection, messageID, decoder);
}
