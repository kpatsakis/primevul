void WebProcessProxy::getPlugins(CoreIPC::Connection*, uint64_t requestID, bool refresh)
{
    pluginWorkQueue().dispatch(bind(&WebProcessProxy::handleGetPlugins, this, requestID, refresh));
}
