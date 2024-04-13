void PpapiPluginProcessHost::RequestPluginChannel(Client* client) {
  base::ProcessHandle process_handle;
  int renderer_id;
  client->GetPpapiChannelInfo(&process_handle, &renderer_id);

  PpapiMsg_CreateChannel* msg = new PpapiMsg_CreateChannel(
      renderer_id, client->OffTheRecord());
  msg->set_unblock(true);
  if (Send(msg)) {
    sent_requests_.push(client);
  } else {
    client->OnPpapiChannelOpened(IPC::ChannelHandle(), 0);
  }
}
