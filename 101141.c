XSSAuditor::XSSAuditor()
    : m_isEnabled(false)
    , m_xssProtection(FilterReflectedXSS)
    , m_didSendValidCSPHeader(false)
    , m_didSendValidXSSProtectionHeader(false)
    , m_state(Uninitialized)
    , m_scriptTagFoundInRequest(false)
    , m_scriptTagNestingLevel(0)
    , m_encoding(UTF8Encoding())
{
}
