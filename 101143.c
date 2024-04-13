bool XSSAuditor::isContainedInRequest(const String& decodedSnippet)
{
    if (decodedSnippet.isEmpty())
        return false;
    if (m_decodedURL.find(decodedSnippet, 0, false) != kNotFound)
        return true;
    if (m_decodedHTTPBodySuffixTree && !m_decodedHTTPBodySuffixTree->mightContain(decodedSnippet))
        return false;
    return m_decodedHTTPBody.find(decodedSnippet, 0, false) != kNotFound;
}
