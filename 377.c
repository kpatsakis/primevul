void WebProcessProxy::getPluginPath(const String& mimeType, const String& urlString, String& pluginPath, uint32_t& pluginLoadPolicy)
{
    MESSAGE_CHECK_URL(urlString);

    String newMimeType = mimeType.lower();

    pluginLoadPolicy = PluginModuleLoadNormally;
    PluginModuleInfo plugin = m_context->pluginInfoStore().findPlugin(newMimeType, KURL(KURL(), urlString));
    if (!plugin.path)
        return;

    pluginLoadPolicy = PluginInfoStore::policyForPlugin(plugin);
    if (pluginLoadPolicy != PluginModuleLoadNormally)
        return;

    pluginPath = plugin.path;
}
