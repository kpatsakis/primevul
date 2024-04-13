void WebProcessProxy::didReceiveMessageOnConnectionWorkQueue(CoreIPC::Connection* connection, CoreIPC::MessageID messageID, CoreIPC::MessageDecoder& decoder, bool& didHandleMessage)
{
    if (decoder.messageReceiverName() == Messages::WebProcessProxy::messageReceiverName())
        didReceiveWebProcessProxyMessageOnConnectionWorkQueue(connection, messageID, decoder, didHandleMessage);
}
