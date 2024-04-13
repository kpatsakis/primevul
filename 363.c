void WebProcessProxy::didGetSitesWithPluginData(const Vector<String>& sites, uint64_t callbackID)
{
    m_context->pluginSiteDataManager()->didGetSitesWithData(sites, callbackID);
}
