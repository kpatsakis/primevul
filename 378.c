void WebProcessProxy::getPluginProcessConnection(const String& pluginPath, uint32_t processType, PassRefPtr<Messages::WebProcessProxy::GetPluginProcessConnection::DelayedReply> reply)
{
    PluginProcessManager::shared().getPluginProcessConnection(m_context->pluginInfoStore(), pluginPath, static_cast<PluginProcess::Type>(processType), reply);
}
