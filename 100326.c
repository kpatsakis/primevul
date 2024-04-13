CSSStyleSheet::CSSStyleSheet(Node* parentNode, const String& href, const KURL& baseURL, const String& charset)
    : StyleSheet(parentNode, href, baseURL)
    , m_doc(parentNode->document())
    , m_namespaces(0)
    , m_charset(charset)
    , m_loadCompleted(false)
    , m_strictParsing(false)
    , m_isUserStyleSheet(false)
    , m_hasSyntacticallyValidCSSHeader(true)
{
}
