bool WebProcessProxy::checkURLReceivedFromWebProcess(const KURL& url)
{

    if (!url.isLocalFile())
        return true;

    if (m_mayHaveUniversalFileReadSandboxExtension)
        return true;

    String path = url.fileSystemPath();
    for (HashSet<String>::const_iterator iter = m_localPathsWithAssumedReadAccess.begin(); iter != m_localPathsWithAssumedReadAccess.end(); ++iter) {
        if (path.startsWith(*iter))
            return true;
    }

    for (WebBackForwardListItemMap::iterator iter = m_backForwardListItemMap.begin(), end = m_backForwardListItemMap.end(); iter != end; ++iter) {
        if (KURL(KURL(), iter->value->url()).fileSystemPath() == path)
            return true;
        if (KURL(KURL(), iter->value->originalURL()).fileSystemPath() == path)
            return true;
    }

    WTFLogAlways("Received an unexpected URL from the web process: '%s'\n", url.string().utf8().data());
    return false;
}
