void WebContentsImpl::OnDidFinishLoad(
    int64 frame_id,
    const GURL& url,
    bool is_main_frame) {
  GURL validated_url(url);
  RenderProcessHost* render_process_host = message_source_->GetProcess();
  RenderViewHost::FilterURL(render_process_host, false, &validated_url);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidFinishLoad(frame_id, validated_url, is_main_frame,
                                  message_source_));
}
