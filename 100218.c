static bool shouldAllowExternalLoad(const KURL& url)
{
    String urlString = url.string();

    if (urlString == "file:///etc/xml/catalog")
        return false;

    if (urlString.startsWith("file:///", false) && urlString.endsWith("/etc/catalog", false))
        return false;

    if (urlString.startsWith("http://www.w3.org/TR/xhtml", false))
        return false;

    if (urlString.startsWith("http://www.w3.org/Graphics/SVG", false))
        return false;

    if (!XMLTokenizerScope::currentDocLoader->doc()->securityOrigin()->canRequest(url)) {
        XMLTokenizerScope::currentDocLoader->printAccessDeniedMessage(url);
        return false;
    }

    return true;
}
