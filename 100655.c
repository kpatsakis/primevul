blink::WebString RenderFrameImpl::doNotTrackValue(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (render_view_->renderer_preferences_.enable_do_not_track)
    return WebString::fromUTF8("1");
  return WebString();
}
