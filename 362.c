void WebProcessProxy::didFinishLaunching(ProcessLauncher* launcher, CoreIPC::Connection::Identifier connectionIdentifier)
{
    ChildProcessProxy::didFinishLaunching(launcher, connectionIdentifier);

#if USE(SECURITY_FRAMEWORK)
    connection()->addQueueClient(&SecItemShimProxy::shared());
#endif

    m_webConnection = WebConnectionToWebProcess::create(this);

    m_context->processDidFinishLaunching(this);

#if PLATFORM(MAC)
    updateProcessSuppressionState();
#endif
}
