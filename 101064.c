bool ChildThread::OnMessageReceived(const IPC::Message& msg) {
  if (mojo_application_->OnMessageReceived(msg))
    return true;

  if (resource_dispatcher_->OnMessageReceived(msg))
    return true;
  if (socket_stream_dispatcher_->OnMessageReceived(msg))
    return true;
  if (websocket_dispatcher_->OnMessageReceived(msg))
    return true;
  if (file_system_dispatcher_->OnMessageReceived(msg))
    return true;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(ChildThread, msg)
    IPC_MESSAGE_HANDLER(ChildProcessMsg_Shutdown, OnShutdown)
#if defined(IPC_MESSAGE_LOG_ENABLED)
    IPC_MESSAGE_HANDLER(ChildProcessMsg_SetIPCLoggingEnabled,
                        OnSetIPCLoggingEnabled)
#endif
    IPC_MESSAGE_HANDLER(ChildProcessMsg_SetProfilerStatus,
                        OnSetProfilerStatus)
    IPC_MESSAGE_HANDLER(ChildProcessMsg_GetChildProfilerData,
                        OnGetChildProfilerData)
    IPC_MESSAGE_HANDLER(ChildProcessMsg_DumpHandles, OnDumpHandles)
#if defined(USE_TCMALLOC)
    IPC_MESSAGE_HANDLER(ChildProcessMsg_GetTcmallocStats, OnGetTcmallocStats)
#endif
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (handled)
    return true;

  if (msg.routing_id() == MSG_ROUTING_CONTROL)
    return OnControlMessageReceived(msg);

  return router_.OnMessageReceived(msg);
}
