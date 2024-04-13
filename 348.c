WebProcessProxy::WebProcessProxy(PassRefPtr<WebContext> context)
    : ChildProcessProxy(this)
    , m_responsivenessTimer(this)
    , m_context(context)
    , m_mayHaveUniversalFileReadSandboxExtension(false)
#if ENABLE(CUSTOM_PROTOCOLS)
    , m_customProtocolManagerProxy(this)
#endif
#if PLATFORM(MAC)
    , m_processSuppressionEnabled(false)
#endif
{
    connect();
}
