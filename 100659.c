void RenderFrameImpl::willStartUsingPeerConnectionHandler(
    blink::WebLocalFrame* frame,
    blink::WebRTCPeerConnectionHandler* handler) {
  DCHECK(!frame_ || frame_ == frame);
#if defined(ENABLE_WEBRTC)
  static_cast<RTCPeerConnectionHandler*>(handler)->associateWithFrame(frame);
#endif
}
