bool RenderProcessHostImpl::OnMessageReceived(const IPC::Message& msg) {

  if (deleting_soon_ || fast_shutdown_started_)
    return false;

  mark_child_process_activity_time();
  if (msg.routing_id() == MSG_ROUTING_CONTROL) {
    IPC_BEGIN_MESSAGE_MAP(RenderProcessHostImpl, msg)
      IPC_MESSAGE_HANDLER(ViewHostMsg_UserMetricsRecordAction,
                          OnUserMetricsRecordAction)
      IPC_MESSAGE_HANDLER(ViewHostMsg_Close_ACK, OnCloseACK)
#if BUILDFLAG(ENABLE_WEBRTC)
      IPC_MESSAGE_HANDLER(AecDumpMsg_RegisterAecDumpConsumer,
                          OnRegisterAecDumpConsumer)
      IPC_MESSAGE_HANDLER(AecDumpMsg_UnregisterAecDumpConsumer,
                          OnUnregisterAecDumpConsumer)
      IPC_MESSAGE_HANDLER(AudioProcessingMsg_Aec3Enabled, OnAec3Enabled)
#endif
    IPC_END_MESSAGE_MAP()

    return true;
  }

  IPC::Listener* listener = listeners_.Lookup(msg.routing_id());
  if (!listener) {
    if (msg.is_sync()) {
      IPC::Message* reply = IPC::SyncMessage::GenerateReply(&msg);
      reply->set_reply_error();
      Send(reply);
    }
    return true;
  }
  return listener->OnMessageReceived(msg);
}
