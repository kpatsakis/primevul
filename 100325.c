CSSStyleSheet::CSSStyleSheet(CSSStyleSheet* parentSheet, const String& href, const KURL& baseURL, const String& charset)
    : StyleSheet(parentSheet, href, baseURL)
    , m_doc(parentSheet ? parentSheet->doc() : 0)
    , m_namespaces(0)
    , m_charset(charset)
    , m_loadCompleted(false)
    , m_strictParsing(!parentSheet || parentSheet->useStrictParsing())
    , m_isUserStyleSheet(parentSheet ? parentSheet->isUserStyleSheet() : false)
    , m_hasSyntacticallyValidCSSHeader(true)
{
}
