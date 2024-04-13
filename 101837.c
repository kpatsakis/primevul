RenderProcessHostImpl::StartRtpDump(
    bool incoming,
    bool outgoing,
    const WebRtcRtpPacketCallback& packet_callback) {
  if (!p2p_socket_dispatcher_host_.get())
    return WebRtcStopRtpDumpCallback();

  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&P2PSocketDispatcherHost::StartRtpDump,
                                         p2p_socket_dispatcher_host_, incoming,
                                         outgoing, packet_callback));

  if (stop_rtp_dump_callback_.is_null()) {
    stop_rtp_dump_callback_ =
        base::Bind(&P2PSocketDispatcherHost::StopRtpDumpOnUIThread,
                   p2p_socket_dispatcher_host_);
  }
  return stop_rtp_dump_callback_;
}
