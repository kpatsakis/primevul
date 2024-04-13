void HTMLAnchorElement::sendPings(const KURL& destinationURL) const
{
    const AtomicString& pingValue = getAttribute(pingAttr);
    if (pingValue.isNull() || !document().settings() || !document().settings()->hyperlinkAuditingEnabled())
        return;

    UseCounter::count(document(), UseCounter::HTMLAnchorElementPingAttribute);

    SpaceSplitString pingURLs(pingValue, SpaceSplitString::ShouldNotFoldCase);
    for (unsigned i = 0; i < pingURLs.size(); i++)
        PingLoader::sendLinkAuditPing(document().frame(), document().completeURL(pingURLs[i]), destinationURL);
}
