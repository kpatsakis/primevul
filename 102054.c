bool SynchronizeVisualPropertiesMessageFilter::OnMessageReceived(
    const IPC::Message& message) {
  IPC_BEGIN_MESSAGE_MAP(SynchronizeVisualPropertiesMessageFilter, message)
    IPC_MESSAGE_HANDLER(FrameHostMsg_SynchronizeVisualProperties,
                        OnSynchronizeFrameHostVisualProperties)
    IPC_MESSAGE_HANDLER(BrowserPluginHostMsg_SynchronizeVisualProperties,
                        OnSynchronizeBrowserPluginVisualProperties)
  IPC_END_MESSAGE_MAP()

  return false;
}
