void WebProcessProxy::addMessageReceiver(CoreIPC::StringReference messageReceiverName, CoreIPC::MessageReceiver* messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, messageReceiver);
}
