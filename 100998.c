bool DocumentThreadableLoader::isAllowedRedirect(const KURL& url) const
{
    if (m_options.crossOriginRequestPolicy == AllowCrossOriginRequests)
        return true;

    return m_sameOriginRequest && getSecurityOrigin()->canRequest(url);
}
