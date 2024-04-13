void RenderFrameImpl::loadURLExternally(
    blink::WebLocalFrame* frame,
    const blink::WebURLRequest& request,
    blink::WebNavigationPolicy policy,
    const blink::WebString& suggested_name) {
  DCHECK(!frame_ || frame_ == frame);
  Referrer referrer(RenderViewImpl::GetReferrerFromRequest(frame, request));
  if (policy == blink::WebNavigationPolicyDownload) {
    render_view_->Send(new ViewHostMsg_DownloadUrl(render_view_->GetRoutingID(),
                                                   request.url(), referrer,
                                                   suggested_name));
  } else {
    OpenURL(frame, request.url(), referrer, policy);
  }
}
