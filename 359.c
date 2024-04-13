void WebProcessProxy::didClearPluginSiteData(uint64_t callbackID)
{
    m_context->pluginSiteDataManager()->didClearSiteData(callbackID);
}
