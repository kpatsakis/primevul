void WebProcessProxy::didReceiveInvalidMessage(CoreIPC::Connection* connection, CoreIPC::StringReference messageReceiverName, CoreIPC::StringReference messageName)
{
    WTFLogAlways("Received an invalid message \"%s.%s\" from the web process.\n", messageReceiverName.toString().data(), messageName.toString().data());

    terminate();

    didClose(connection);
}
