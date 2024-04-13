void MediaStreamDispatcherHost::SetCapturingLinkSecured(
    int32_t session_id,
    blink::MediaStreamType type,
    bool is_secure) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  media_stream_manager_->SetCapturingLinkSecured(render_process_id_, session_id,
                                                 type, is_secure);
}
